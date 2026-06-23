#!/usr/bin/env node
'use strict';

// notleetcode — a tiny local LeetCode-style runner.
// Plain Node, no npm dependencies. Serves the web/ UI and runs user code
// against per-problem hidden test harnesses using python3 / gcc / g++.

const http = require('http');
const fs = require('fs');
const os = require('os');
const path = require('path');
const { spawnSync } = require('child_process');

const ROOT = __dirname;
const WEB_DIR = path.join(ROOT, 'web');
const PROBLEMS_DIR = path.join(ROOT, 'problems');
const SOLUTIONS_DIR = path.join(ROOT, 'solutions');
const PROGRESS_FILE = path.join(ROOT, 'progress.json');

const PORT = process.env.PORT || 5151;
const RUN_TIMEOUT_MS = 12000;

// ---------------------------------------------------------------------------
// Language configuration
// ---------------------------------------------------------------------------
const LANGS = {
  python: {
    ext: 'py',
    solutionFile: 'solution.py',
    harnessFile: 'main.py',
    compile: null,
    run: ['python3', 'main.py'],
    cmEditorMode: 'python',
  },
  c: {
    ext: 'c',
    solutionFile: 'solution.c',
    harnessFile: 'main.c',
    // main.c #includes "solution.c", so we only compile main.c.
    compile: ['gcc', '-std=c11', '-O0', '-g', 'main.c', '-o', 'prog', '-lm'],
    run: ['./prog'],
    cmEditorMode: 'text/x-csrc',
  },
  cpp: {
    ext: 'cpp',
    solutionFile: 'solution.cpp',
    harnessFile: 'main.cpp',
    compile: ['g++', '-std=c++17', '-O0', '-g', 'main.cpp', '-o', 'prog'],
    run: ['./prog'],
    cmEditorMode: 'text/x-c++src',
  },
};

const RESULT_BEGIN = '__TESTS_BEGIN__';
const RESULT_END = '__TESTS_END__';

// Every problem offers the same languages, regardless of what its meta says.
const OFFERED_LANGUAGES = ['python', 'c'];

// Support files copied into the sandbox alongside the user's solution.
const SUPPORT_FILE = { python: 'nlc.py', c: 'nlc.h', cpp: 'nlc.h' };

// ---------------------------------------------------------------------------
// Problem loading
// ---------------------------------------------------------------------------
function listProblemDirs() {
  if (!fs.existsSync(PROBLEMS_DIR)) return [];
  return fs
    .readdirSync(PROBLEMS_DIR, { withFileTypes: true })
    .filter((d) => d.isDirectory() && !d.name.startsWith('_'))
    .map((d) => d.name);
}

function readIfExists(p) {
  try {
    return fs.readFileSync(p, 'utf8');
  } catch (e) {
    return null;
  }
}

function loadProblemMeta(id) {
  const dir = path.join(PROBLEMS_DIR, id);
  const metaRaw = readIfExists(path.join(dir, 'meta.json'));
  if (!metaRaw) return null;
  let meta;
  try {
    meta = JSON.parse(metaRaw);
  } catch (e) {
    console.error(`Bad meta.json for ${id}: ${e.message}`);
    return null;
  }
  meta.id = id;
  return meta;
}

function defaultStub(lang) {
  if (lang === 'c') return '#include <stdio.h>\n\n/* Write your solution here. */\n';
  if (lang === 'cpp') return '#include <bits/stdc++.h>\nusing namespace std;\n\n// Write your solution here.\n';
  return '# Write your solution here.\n';
}

function loadProblemFull(id) {
  const dir = path.join(PROBLEMS_DIR, id);
  const meta = loadProblemMeta(id);
  if (!meta) return null;
  meta.description = readIfExists(path.join(dir, 'description.md')) || '_No description yet._';
  meta.starter = {};
  meta.solution = {};
  meta.hasHarness = {};
  // Offer C and Python on every problem, ignoring meta.languages.
  const langs = OFFERED_LANGUAGES;
  meta.languages = langs;
  for (const lang of langs) {
    const cfg = LANGS[lang];
    if (!cfg) continue;
    const starter = readIfExists(path.join(dir, `starter.${cfg.ext}`));
    meta.starter[lang] = starter != null ? starter : defaultStub(lang);
    const solution = readIfExists(path.join(dir, `solution_ref.${cfg.ext}`));
    if (solution != null) meta.solution[lang] = solution;
    meta.hasHarness[lang] = fs.existsSync(path.join(dir, `harness.${cfg.ext}`));
  }
  meta.editorModes = {};
  for (const lang of langs) meta.editorModes[lang] = LANGS[lang] ? LANGS[lang].cmEditorMode : 'text/plain';
  return meta;
}

function allProblemsOrdered() {
  const metas = listProblemDirs()
    .map(loadProblemMeta)
    .filter(Boolean);
  metas.sort((a, b) => (a.stackIndex || 0) - (b.stackIndex || 0));
  return metas;
}

// ---------------------------------------------------------------------------
// Progress
// ---------------------------------------------------------------------------
function loadProgress() {
  const raw = readIfExists(PROGRESS_FILE);
  if (!raw) return {};
  try {
    return JSON.parse(raw);
  } catch (e) {
    return {};
  }
}

function saveProgress(p) {
  fs.writeFileSync(PROGRESS_FILE, JSON.stringify(p, null, 2) + '\n');
}

function setStatus(id, status, lang) {
  const p = loadProgress();
  const cur = p[id] || {};
  // Never downgrade a solved problem to attempted automatically.
  if (cur.status === 'solved' && status === 'attempted') {
    status = 'solved';
  }
  p[id] = {
    status,
    lastLanguage: lang || cur.lastLanguage || null,
    updated: new Date().toISOString(),
  };
  saveProgress(p);
  return p[id];
}

// ---------------------------------------------------------------------------
// Saved solutions
// ---------------------------------------------------------------------------
function solutionPath(id, lang) {
  const cfg = LANGS[lang];
  const ext = cfg ? cfg.ext : 'txt';
  return path.join(SOLUTIONS_DIR, `${id}.${ext}`);
}

function saveSolution(id, lang, code) {
  if (!fs.existsSync(SOLUTIONS_DIR)) fs.mkdirSync(SOLUTIONS_DIR, { recursive: true });
  fs.writeFileSync(solutionPath(id, lang), code);
}

function loadSolution(id, lang) {
  return readIfExists(solutionPath(id, lang));
}

// ---------------------------------------------------------------------------
// Code execution
// ---------------------------------------------------------------------------
function runUserCode(id, lang, code) {
  const cfg = LANGS[lang];
  if (!cfg) return { ok: false, error: `Unsupported language: ${lang}` };

  const dir = path.join(PROBLEMS_DIR, id);
  const harness = readIfExists(path.join(dir, `harness.${cfg.ext}`));
  if (harness == null) {
    return { ok: false, error: `No test harness for this problem in ${lang} yet.` };
  }

  const tmp = fs.mkdtempSync(path.join(os.tmpdir(), `nlc-${id}-`));
  try {
    fs.writeFileSync(path.join(tmp, cfg.solutionFile), code);
    fs.writeFileSync(path.join(tmp, cfg.harnessFile), harness);

    // Copy the shared harness support file into the sandbox.
    const support = SUPPORT_FILE[lang];
    if (support) {
      const src = readIfExists(path.join(PROBLEMS_DIR, '_lib', support));
      if (src != null) fs.writeFileSync(path.join(tmp, support), src);
    }

    // Compile step (C / C++)
    if (cfg.compile) {
      const c = spawnSync(cfg.compile[0], cfg.compile.slice(1), {
        cwd: tmp,
        encoding: 'utf8',
        timeout: RUN_TIMEOUT_MS,
      });
      if (c.error && c.error.code === 'ENOENT') {
        return { ok: false, error: `Compiler not found: ${cfg.compile[0]}. Install it and retry.` };
      }
      if (c.status !== 0) {
        return { ok: false, stage: 'compile', compileError: (c.stderr || '') + (c.stdout || '') };
      }
    }

    // Run step
    const r = spawnSync(cfg.run[0], cfg.run.slice(1), {
      cwd: tmp,
      encoding: 'utf8',
      timeout: RUN_TIMEOUT_MS,
      maxBuffer: 16 * 1024 * 1024,
    });

    if (r.error && r.error.code === 'ENOENT') {
      return { ok: false, error: `Runtime not found: ${cfg.run[0]}. Install it and retry.` };
    }
    if (r.error && r.error.code === 'ETIMEDOUT') {
      return { ok: false, stage: 'run', timeout: true, stdout: r.stdout || '', stderr: r.stderr || '' };
    }

    const stdout = r.stdout || '';
    const stderr = r.stderr || '';
    const parsed = parseResults(stdout);
    if (!parsed) {
      return {
        ok: false,
        stage: 'run',
        error: 'Could not read test results from program output.',
        stdout,
        stderr,
        exitCode: r.status,
      };
    }
    const userStdout = stripResultBlock(stdout);
    const allPassed = parsed.results.length > 0 && parsed.results.every((t) => t.passed);
    return {
      ok: true,
      results: parsed.results,
      allPassed,
      stdout: userStdout,
      stderr,
      exitCode: r.status,
    };
  } finally {
    try {
      fs.rmSync(tmp, { recursive: true, force: true });
    } catch (e) {
      /* ignore */
    }
  }
}

function parseResults(stdout) {
  const b = stdout.indexOf(RESULT_BEGIN);
  const e = stdout.indexOf(RESULT_END);
  if (b === -1 || e === -1 || e < b) return null;
  const json = stdout.slice(b + RESULT_BEGIN.length, e).trim();
  try {
    const obj = JSON.parse(json);
    if (!obj || !Array.isArray(obj.results)) return null;
    return obj;
  } catch (err) {
    return null;
  }
}

function stripResultBlock(stdout) {
  const b = stdout.indexOf(RESULT_BEGIN);
  const e = stdout.indexOf(RESULT_END);
  if (b === -1 || e === -1) return stdout.trim();
  const before = stdout.slice(0, b);
  const after = stdout.slice(e + RESULT_END.length);
  return (before + after).trim();
}

// ---------------------------------------------------------------------------
// HTTP plumbing
// ---------------------------------------------------------------------------
const MIME = {
  '.html': 'text/html; charset=utf-8',
  '.js': 'text/javascript; charset=utf-8',
  '.css': 'text/css; charset=utf-8',
  '.json': 'application/json; charset=utf-8',
  '.svg': 'image/svg+xml',
};

function sendJSON(res, code, obj) {
  const body = JSON.stringify(obj);
  res.writeHead(code, { 'Content-Type': 'application/json; charset=utf-8' });
  res.end(body);
}

function serveStatic(req, res) {
  let urlPath = decodeURIComponent(req.url.split('?')[0]);
  if (urlPath === '/') urlPath = '/index.html';
  const filePath = path.join(WEB_DIR, path.normalize(urlPath));
  if (!filePath.startsWith(WEB_DIR)) {
    res.writeHead(403);
    res.end('Forbidden');
    return;
  }
  fs.readFile(filePath, (err, data) => {
    if (err) {
      res.writeHead(404);
      res.end('Not found');
      return;
    }
    const ext = path.extname(filePath).toLowerCase();
    res.writeHead(200, { 'Content-Type': MIME[ext] || 'application/octet-stream' });
    res.end(data);
  });
}

function readBody(req) {
  return new Promise((resolve, reject) => {
    let data = '';
    req.on('data', (chunk) => {
      data += chunk;
      if (data.length > 8 * 1024 * 1024) reject(new Error('Body too large'));
    });
    req.on('end', () => resolve(data));
    req.on('error', reject);
  });
}

const server = http.createServer(async (req, res) => {
  const url = req.url.split('?')[0];

  try {
    // ----- API -----
    if (url === '/api/problems' && req.method === 'GET') {
      const progress = loadProgress();
      const problems = allProblemsOrdered().map((m) => ({
        id: m.id,
        title: m.title,
        phase: m.phase,
        phaseName: m.phaseName,
        difficulty: m.difficulty,
        tags: m.tags || [],
        languages: OFFERED_LANGUAGES,
        stackIndex: m.stackIndex,
        authored: !!m.authored,
        status: (progress[m.id] && progress[m.id].status) || 'todo',
      }));
      return sendJSON(res, 200, { problems });
    }

    if (url.startsWith('/api/problem/') && req.method === 'GET') {
      const id = url.slice('/api/problem/'.length);
      const prob = loadProblemFull(id);
      if (!prob) return sendJSON(res, 404, { error: 'Unknown problem' });
      const progress = loadProgress();
      prob.progress = progress[id] || { status: 'todo' };
      // Attach saved solutions per language
      prob.saved = {};
      for (const lang of prob.languages || ['python']) {
        const s = loadSolution(id, lang);
        if (s != null) prob.saved[lang] = s;
      }
      return sendJSON(res, 200, prob);
    }

    if (url === '/api/run' && req.method === 'POST') {
      const body = JSON.parse(await readBody(req));
      const { id, language, code } = body;
      if (!id || !language || code == null) return sendJSON(res, 400, { error: 'Missing fields' });
      saveSolution(id, language, code);
      const result = runUserCode(id, language, code);
      if (result.ok && result.allPassed) setStatus(id, 'solved', language);
      else setStatus(id, 'attempted', language);
      const progress = loadProgress();
      result.status = progress[id].status;
      return sendJSON(res, 200, result);
    }

    if (url === '/api/save' && req.method === 'POST') {
      const body = JSON.parse(await readBody(req));
      const { id, language, code } = body;
      if (!id || !language || code == null) return sendJSON(res, 400, { error: 'Missing fields' });
      saveSolution(id, language, code);
      setStatus(id, 'attempted', language);
      return sendJSON(res, 200, { ok: true });
    }

    if (url === '/api/status' && req.method === 'POST') {
      const body = JSON.parse(await readBody(req));
      const { id, status, language } = body;
      if (!id || !status) return sendJSON(res, 400, { error: 'Missing fields' });
      const st = setStatus(id, status, language);
      return sendJSON(res, 200, { ok: true, status: st });
    }

    // ----- Static -----
    if (req.method === 'GET') return serveStatic(req, res);

    res.writeHead(405);
    res.end('Method not allowed');
  } catch (err) {
    console.error(err);
    sendJSON(res, 500, { error: err.message });
  }
});

server.listen(PORT, () => {
  console.log(`\n  notleetcode running →  http://localhost:${PORT}\n`);
});
