'use strict';

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------
let problems = [];        // all problems from /api/problems (every tab)
let current = null;       // full problem object
let currentLang = null;
let allowedTabs = ['general', 'firmware', 'robotics'];
let activeTab = localStorage.getItem('nlc-tab') || 'general';
let activeUser = '';
let saveQueue = Promise.resolve(true);
let pendingSaves = 0;
let loadingEditor = false;
let changingProblem = false;

function tabProblems() {
  // Order by phase, then stackIndex, so the sidebar's phase grouping never
  // emits a duplicate section header (a problem whose stackIndex puts it out
  // of phase order would otherwise re-open an already-closed section).
  return problems
    .filter((p) => (p.tab || 'general') === activeTab)
    .sort((a, b) => (a.phase - b.phase) || (a.stackIndex - b.stackIndex));
}

function setTab(tab) {
  activeTab = tab;
  localStorage.setItem('nlc-tab', tab);
  document.querySelectorAll('.tab-btn').forEach((b) =>
    b.classList.toggle('active', b.dataset.tab === tab));
  renderSidebar();
  renderOverall();
  const list = tabProblems();
  const first = list.find((p) => p.status !== 'solved') || list[0];
  if (first) openProblem(first.id);
}
let editor = null;
let saveTimer = null;

const PHASE_NAMES = {};   // filled from problem data

// ---------------------------------------------------------------------------
// Autocomplete
// ---------------------------------------------------------------------------
const C_KEYWORDS = [
  'auto','break','case','char','const','continue','default','do','double','else',
  'enum','extern','float','for','goto','if','inline','int','long','register',
  'restrict','return','short','signed','sizeof','static','struct','switch',
  'typedef','union','unsigned','void','volatile','while',
  'NULL','true','false','bool',
  'malloc','calloc','realloc','free',
  'printf','fprintf','sprintf','snprintf','scanf','sscanf',
  'strlen','strcpy','strncpy','strcat','strncat','strcmp','strncmp',
  'strchr','strrchr','strstr','strtol','strtod','strtok',
  'memcpy','memmove','memset','memcmp',
  'fopen','fclose','fread','fwrite','fgets','fputs','fgetc','fputc','puts','putchar','getchar',
  'exit','abort','assert',
  'INT_MAX','INT_MIN','UINT_MAX','SIZE_MAX',
  'size_t','ssize_t',
  'uint8_t','uint16_t','uint32_t','uint64_t',
  'int8_t','int16_t','int32_t','int64_t',
  'EOF',
];

function editorHint(cm) {
  const mode = cm.getOption('mode');
  const cur = cm.getCursor();
  const lineText = cm.getLine(cur.line);
  let start = cur.ch;
  while (start > 0 && /\w/.test(lineText[start - 1])) start--;
  const prefix = lineText.slice(start, cur.ch);
  if (!prefix) return null;

  const seen = new Set();
  const list = [];
  function add(w) {
    if (w !== prefix && w.startsWith(prefix) && !seen.has(w)) {
      seen.add(w);
      list.push(w);
    }
  }

  cm.eachLine((lh) => {
    for (const m of lh.text.matchAll(/\b[a-zA-Z_]\w*\b/g)) add(m[0]);
  });

  if (mode === 'python' || mode === 'text/x-python') {
    try { const ph = CodeMirror.hint.python(cm); if (ph) ph.list.forEach(add); } catch (_) {}
  } else if (mode === 'text/x-csrc' || mode === 'text/x-c++src') {
    C_KEYWORDS.forEach(add);
  }

  if (!list.length) return null;
  list.sort();
  return { list, from: { line: cur.line, ch: start }, to: { line: cur.line, ch: cur.ch } };
}

// ---------------------------------------------------------------------------
// Boot
// ---------------------------------------------------------------------------
window.addEventListener('DOMContentLoaded', init);

async function init() {
  editor = CodeMirror(document.getElementById('editor-host'), {
    value: '',
    mode: 'python',
    theme: 'material-darker',
    lineNumbers: true,
    indentUnit: 4,
    tabSize: 4,
    indentWithTabs: false, // soft tabs — VSCode-style
    smartIndent: true,
    autoCloseBrackets: true,
    matchBrackets: true,
    styleActiveLine: true,
    extraKeys: {
      'Cmd-Enter': runTests,
      'Ctrl-Enter': runTests,
      'Cmd-S': () => { saveNow(); },
      'Ctrl-S': () => { saveNow(); },
      'Cmd-/': (cm) => cm.toggleComment({ indent: true }),
      'Ctrl-/': (cm) => cm.toggleComment({ indent: true }),
      'Ctrl-Space': (cm) => cm.showHint({ hint: editorHint, completeSingle: false }),
      // Tab: show completions when cursor is mid-word, otherwise insert spaces.
      Tab: (cm) => {
        if (cm.somethingSelected()) { cm.indentSelection('add'); return; }
        const cur = cm.getCursor();
        const lineText = cm.getLine(cur.line);
        let wStart = cur.ch;
        while (wStart > 0 && /\w/.test(lineText[wStart - 1])) wStart--;
        if (wStart < cur.ch) {
          cm.showHint({ hint: editorHint, completeSingle: false });
        } else {
          cm.replaceSelection(' '.repeat(cm.getOption('indentUnit')), 'end', '+input');
        }
      },
      'Shift-Tab': (cm) => cm.indentSelection('subtract'),
      // Backspace deletes a full indent's worth of spaces when sitting in
      // leading whitespace — so removing indentation feels like VSCode.
      Backspace: (cm) => softTabBackspace(cm),
    },
  });
  editor.on('change', onEditorChange);
  editor.on('inputRead', (cm, event) => {
    if (!cm.state.completionActive && event.origin === '+input') {
      const ch = (event.text || [])[0];
      if (ch && /\w/.test(ch)) cm.showHint({ hint: editorHint, completeSingle: false });
    }
  });

  // Disable macOS autocorrect/autocapitalize on the editor input field.
  const inp = editor.getInputField();
  inp.setAttribute('autocorrect', 'off');
  inp.setAttribute('autocapitalize', 'off');
  inp.setAttribute('spellcheck', 'false');

  wireButtons();
  const [, cfg] = await Promise.all([loadProblems(), fetch('/api/config').then((r) => r.json())]);
  allowedTabs = cfg.tabs || allowedTabs;
  activeUser = cfg.user || '';
  await recoverBrowserAttempts();
  const cleanURL = new URL(location.href);
  cleanURL.searchParams.delete('practice');
  cleanURL.searchParams.delete('attempt');
  history.replaceState(null, '', cleanURL);
  document.addEventListener('visibilitychange', () => { if (document.hidden) saveNow(); });
  window.addEventListener('beforeunload', (event) => {
    if (pendingSaves || (current && editor.getValue() !== current.saved[currentLang])) {
      saveNow();
      event.preventDefault();
      event.returnValue = '';
    }
  });

  // Hide tab buttons the user isn't allowed to see.
  document.querySelectorAll('.tab-btn').forEach((b) => {
    if (!allowedTabs.includes(b.dataset.tab)) b.style.display = 'none';
  });

  // Clamp activeTab to an allowed tab.
  if (!allowedTabs.includes(activeTab)) {
    activeTab = allowedTabs[0];
    localStorage.setItem('nlc-tab', activeTab);
  }

  // Restore active tab UI state.
  document.querySelectorAll('.tab-btn').forEach((b) =>
    b.classList.toggle('active', b.dataset.tab === activeTab));

  // Open last-opened (from hash) or first unsolved in active tab.
  const fromHash = location.hash.slice(1);
  let target = problems.find((p) => p.id === fromHash);
  if (target) {
    // Switch to the tab that owns this problem, only if allowed.
    const ownerTab = target.tab || 'general';
    if (ownerTab !== activeTab && allowedTabs.includes(ownerTab)) {
      activeTab = ownerTab;
      localStorage.setItem('nlc-tab', activeTab);
      document.querySelectorAll('.tab-btn').forEach((b) =>
        b.classList.toggle('active', b.dataset.tab === activeTab));
    }
  } else {
    const list = tabProblems();
    target = list.find((p) => p.status !== 'solved') || list[0];
  }
  if (target) await openProblem(target.id);
  renderSidebar();
  renderOverall();
}

function wireButtons() {
  document.getElementById('run-btn').onclick = runTests;
  document.getElementById('reset-btn').onclick = resetToStarter;
  document.getElementById('prev-btn').onclick = () => navigate(-1);
  document.getElementById('next-btn').onclick = () => navigate(1);
  document.getElementById('lang-select').onchange = (e) => switchLang(e.target.value);
  document.getElementById('toggle-solution').onclick = toggleSolution;
  document.getElementById('flag-btn').onclick = toggleFlag;
  document.getElementById('copy-btn').onclick = copyForLLM;
  document.querySelectorAll('.tab-btn').forEach((btn) => {
    btn.onclick = () => setTab(btn.dataset.tab);
  });
  document.querySelectorAll('.info-tab').forEach((btn) => {
    btn.onclick = () => switchInfoPanel(btn.dataset.panel);
  });
}

function switchInfoPanel(panel) {
  document.querySelectorAll('.info-tab').forEach((b) =>
    b.classList.toggle('active', b.dataset.panel === panel));
  document.querySelectorAll('.info-panel').forEach((p) =>
    p.classList.toggle('hidden', p.id !== 'panel-' + panel));
}

async function copyForLLM() {
  if (!current) return;
  const lang = currentLang || 'python';
  const code = editor.getValue();
  const text = [
    `# ${current.title}`,
    '',
    current.description || '',
    '',
    '---',
    '',
    '```' + (lang === 'python' ? 'python' : 'c'),
    code,
    '```',
  ].join('\n');

  try {
    await navigator.clipboard.writeText(text);
  } catch {
    // Fallback for non-secure contexts
    const ta = document.createElement('textarea');
    ta.value = text;
    ta.style.position = 'fixed';
    ta.style.opacity = '0';
    document.body.appendChild(ta);
    ta.select();
    document.execCommand('copy');
    document.body.removeChild(ta);
  }

  const btn = document.getElementById('copy-btn');
  const orig = btn.textContent;
  btn.textContent = 'Copied!';
  btn.classList.add('copied');
  setTimeout(() => { btn.textContent = orig; btn.classList.remove('copied'); }, 1500);
}

// ---------------------------------------------------------------------------
// Problem list / sidebar
// ---------------------------------------------------------------------------
async function loadProblems() {
  const res = await fetch('/api/problems');
  const data = await res.json();
  problems = data.problems;
  renderSidebar();
  renderOverall();
}

function renderSidebar() {
  const host = document.getElementById('problem-list');
  host.innerHTML = '';
  let lastPhase = null;
  for (const p of tabProblems()) {
    if (p.phase !== lastPhase) {
      lastPhase = p.phase;
      const label = document.createElement('div');
      if (p.phase === 0) {
        label.className = 'phase-label phase-label-featured';
        label.textContent = p.phaseName || '';
      } else {
        label.className = 'phase-label';
        label.textContent = `Phase ${p.phase} — ${p.phaseName || ''}`;
      }
      host.appendChild(label);
    }
    const item = document.createElement('div');
    item.className = 'prob-item' + (p.authored ? '' : ' unauthored') + (current && p.id === current.id ? ' active' : '');
    item.dataset.id = p.id;
    item.innerHTML = `
      <span class="status-dot ${p.status}"></span>
      <span class="num">${p.stackIndex}</span>
      <span class="name">${escapeHtml(p.title)}</span>
      <span class="flag-mark${p.flagged ? ' on' : ''}" title="Flagged for review">⚑</span>
      <span class="lang-tag">${(p.defaultLanguage || p.languages[0] || 'py').replace('python', 'py').replace('cpp', 'c++')}</span>`;
    item.onclick = () => openProblem(p.id);
    host.appendChild(item);
  }
}

function renderOverall() {
  const list = tabProblems();
  const solved = list.filter((p) => p.status === 'solved').length;
  const attempted = list.filter((p) => p.status === 'attempted').length;
  document.getElementById('overall-progress').textContent =
    `${solved} solved · ${attempted} in progress · ${list.length} total`;
}

function highlightActive() {
  document.querySelectorAll('.prob-item').forEach((el) => {
    el.classList.toggle('active', current && el.dataset.id === current.id);
  });
}

// ---------------------------------------------------------------------------
// Open / render a problem
// ---------------------------------------------------------------------------
async function openProblem(id) {
  if (changingProblem) return;
  changingProblem = true;
  editor.setOption('readOnly', true);
  try {
    if (!await saveNow()) return;
    const res = await fetch('/api/problem/' + id);
    if (!res.ok) return;
    current = await res.json();
    location.hash = id;

    document.getElementById('prob-index').textContent = current.phase === 0
      ? `#${current.stackIndex} · ${current.phaseName}`
      : `#${current.stackIndex} · Phase ${current.phase}`;
    document.getElementById('prob-title').textContent = current.title;
    const diff = document.getElementById('prob-diff');
    diff.textContent = current.difficulty || '';
    diff.className = 'badge ' + (current.difficulty || '');
    const co = document.getElementById('prob-company');
    if (current.company) {
      co.textContent = current.company;
      co.classList.remove('hidden');
    } else {
      co.classList.add('hidden');
    }
    renderStatusBadge(current.progress ? current.progress.status : 'todo');

    const tagHost = document.getElementById('prob-tags');
    tagHost.innerHTML = (current.tags || []).map((t) => `<span class="tag">${escapeHtml(t)}</span>`).join('');

    document.getElementById('prob-desc').innerHTML = marked.parse(current.description || '');

    // Language selector
    const sel = document.getElementById('lang-select');
    sel.innerHTML = (current.languages || ['python'])
      .map((l) => `<option value="${l}">${langLabel(l)}</option>`)
      .join('');
    currentLang = (current.languages.includes(current.progress?.lastLanguage) && current.progress.lastLanguage) || current.defaultLanguage ||
      (current.languages && current.languages[0]) || 'python';
    sel.value = currentLang;

    renderFlag(current.progress ? current.progress.flagged : false);

    loadEditorForLang();
    setupSolutionBlock();
    clearResults();
    switchInfoPanel('problem');
    highlightActive();
  } finally {
    changingProblem = false;
    editor.setOption('readOnly', false);
  }
}

function renderFlag(flagged) {
  const btn = document.getElementById('flag-btn');
  btn.classList.toggle('on', !!flagged);
  btn.title = flagged ? 'Flagged for review — click to unflag' : 'Flag for review later';
}

async function toggleFlag() {
  if (!current) return;
  const res = await fetch('/api/flag', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ id: current.id }),
  });
  if (!res.ok) return;
  const data = await res.json();
  if (current.progress) current.progress.flagged = data.flagged;
  renderFlag(data.flagged);
  // Keep the sidebar mark and the in-memory list in sync.
  const p = problems.find((x) => x.id === current.id);
  if (p) p.flagged = data.flagged;
  const mark = document.querySelector(`.prob-item[data-id="${current.id}"] .flag-mark`);
  if (mark) mark.classList.toggle('on', data.flagged);
}

function renderStatusBadge(status) {
  const el = document.getElementById('prob-status');
  if (status === 'solved') { el.textContent = '✓ Solved'; el.className = 'badge solved'; }
  else if (status === 'attempted') { el.textContent = '◐ In progress'; el.className = 'badge attempted'; }
  else { el.textContent = 'Not started'; el.className = 'badge'; }
}

async function switchLang(lang) {
  if (changingProblem) return;
  changingProblem = true;
  editor.setOption('readOnly', true);
  try {
    if (!await saveNow()) {
      document.getElementById('lang-select').value = currentLang;
      return;
    }
    currentLang = lang;
    loadEditorForLang();
    setupSolutionBlock();
    clearResults();
  } finally {
    changingProblem = false;
    editor.setOption('readOnly', false);
  }
}

function loadEditorForLang() {
  const mode = (current.editorModes && current.editorModes[currentLang]) || 'python';
  editor.setOption('mode', mode);
  const saved = current.saved[currentLang];
  const code = saved ?? current.starter?.[currentLang] ?? '';
  // The starter is the effective baseline until the first edit is saved.
  current.saved[currentLang] = code;
  loadingEditor = true;
  editor.setValue(code);
  loadingEditor = false;
  setSaveState(saved != null ? 'Saved to project' : 'Autosave ready');
  const extension = { python:'py', c:'c', cpp:'cpp' }[currentLang];
  document.getElementById('save-state').title = `solutions/${activeUser}/${current.id}.${extension}`;
}

function resetToStarter() {
  if (!current) return;
  editor.setValue(current.starter?.[currentLang] || '');
  clearResults();
  saveNow(true);
}

function setupSolutionBlock() {
  const block = document.getElementById('solution-block');
  const code = document.getElementById('solution-code');
  const btn = document.getElementById('toggle-solution');
  const ref = current.solution && current.solution[currentLang];
  if (ref) {
    block.classList.remove('hidden');
    code.textContent = ref;
    code.classList.add('hidden');
    btn.textContent = '▸ Show reference solution';
  } else {
    block.classList.add('hidden');
  }
}

function toggleSolution() {
  const code = document.getElementById('solution-code');
  const btn = document.getElementById('toggle-solution');
  const hidden = code.classList.toggle('hidden');
  btn.textContent = (hidden ? '▸ Show' : '▾ Hide') + ' reference solution';
}

// ---------------------------------------------------------------------------
// Saving
// ---------------------------------------------------------------------------
function onEditorChange() {
  if (loadingEditor || !current) return;
  setSaveState('Editing…');
  renderStatusBadge('attempted');
  if (saveTimer) clearTimeout(saveTimer);
  saveTimer = setTimeout(saveNow, 800);
}

// Serialize saves and runs so a slower response cannot overwrite a newer edit.
function enqueueSave(task) {
  pendingSaves++;
  const job = saveQueue.then(task).finally(() => { pendingSaves--; });
  saveQueue = job.catch(() => false);
  return saveQueue;
}

function rememberSaved(problem, lang, code, status) {
  problem.saved[lang] = code;
  problem.progress = { ...problem.progress, status, lastLanguage:lang };
  bumpStatus(problem.id, status);
  if (current === problem && currentLang === lang) {
    const unchanged = editor.getValue() === code;
    setSaveState(unchanged ? 'Saved to project' : 'Editing…');
    renderStatusBadge(unchanged ? status : 'attempted');
  }
}

function saveNow(force = false) {
  if (saveTimer) { clearTimeout(saveTimer); saveTimer = null; }
  if (!current) return Promise.resolve(true);
  const problem = current, lang = currentLang, code = editor.getValue();
  return enqueueSave(async () => {
    if (!force && code === problem.saved[lang]) {
      if (current === problem && currentLang === lang && editor.getValue() === code) {
        setSaveState('No unsaved changes');
        renderStatusBadge(problem.progress?.status || 'todo');
      }
      return true;
    }
    try {
      if (current === problem) setSaveState('Saving…');
      const res = await fetch('/api/save', {
        method:'POST', headers:{ 'Content-Type':'application/json' },
        body:JSON.stringify({ id:problem.id, language:lang, code }),
      });
      if (!res.ok) throw new Error('Save failed');
      rememberSaved(problem, lang, code, 'attempted');
      return true;
    } catch (error) {
      if (current === problem) setSaveState('Not saved — retry with ⌘/Ctrl+S');
      return false;
    }
  });
}

function setSaveState(txt) {
  document.getElementById('save-state').textContent = txt;
}

// One-time recovery of the old browser-only mode. Keep only the newest draft
// for each question/language; remove old keys only after the file is saved.
async function recoverBrowserAttempts() {
  const latest = new Map(), prefix = `nlc-practice:${activeUser}:`;
  let recovered = 0, conflicts = 0;
  try {
    for (let i = 0; i < localStorage.length; i++) {
      const key = localStorage.key(i);
      if (!key.startsWith(prefix)) continue;
      const [attempt, id, language] = key.slice(prefix.length).split(':');
      if (!problems.some((p) => p.id === id && p.languages.includes(language))) continue;
      const group = `${id}:${language}`, previous = latest.get(group);
      const draft = { id, language, code:localStorage.getItem(key), at:Number(attempt) || 0, keys:[] };
      if (!previous || draft.at >= previous.at) {
        draft.keys = [...(previous?.keys || []), key];
        latest.set(group, draft);
      } else previous.keys.push(key);
    }
    for (const draft of latest.values()) {
      const res = await fetch('/api/save', {
        method:'POST', headers:{ 'Content-Type':'application/json' },
        body:JSON.stringify({ ...draft, recover:true }),
      });
      if (res.status === 409) { conflicts++; continue; }
      if (!res.ok) throw new Error('Recovery failed');
      draft.keys.forEach((key) => localStorage.removeItem(key));
      recovered++;
    }
    if (latest.size) {
      if (recovered) await loadProblems();
      document.getElementById('recovery-message').textContent =
        (recovered ? `Recovered ${recovered} browser attempt${recovered === 1 ? '' : 's'} into your project. ` : '') +
        (conflicts ? 'Existing project code kept; conflicting old browser drafts retained. ' : '');
    }
  } catch (error) {
    document.getElementById('recovery-message').textContent = 'Could not recover browser attempts. They are still in this browser; reload to retry. ';
  }
}

// ---------------------------------------------------------------------------
// Running tests
// ---------------------------------------------------------------------------
async function runTests() {
  if (!current) return;
  if (saveTimer) { clearTimeout(saveTimer); saveTimer = null; }
  const problem = current, lang = currentLang, code = editor.getValue();
  const runBtn = document.getElementById('run-btn');
  if (runBtn.disabled) return;
  runBtn.disabled = true;
  const summary = document.getElementById('results-summary');
  summary.className = 'results-summary running';
  summary.textContent = 'Running…';
  document.getElementById('results-detail').innerHTML = '';
  await enqueueSave(async () => {
    try {
      const res = await fetch('/api/run', {
        method:'POST', headers:{ 'Content-Type':'application/json' },
        body:JSON.stringify({ id:problem.id, language:lang, code }),
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.error || 'Run failed');
      rememberSaved(problem, lang, code, data.status);
      if (current === problem && currentLang === lang) {
        if (editor.getValue() === code) renderResults(data);
        else {
          summary.className = 'results-summary';
          summary.textContent = 'Code changed during the run. Run again to test your latest work.';
        }
        switchInfoPanel('results');
      }
      return true;
    } catch (error) {
      if (current === problem && currentLang === lang) {
        summary.className = 'results-summary fail';
        summary.textContent = 'Error: ' + error.message;
        setSaveState('Save not confirmed — retry with ⌘/Ctrl+S');
        switchInfoPanel('results');
      }
      return false;
    } finally {
      runBtn.disabled = false;
    }
  });
}

function renderResults(data) {
  const summary = document.getElementById('results-summary');
  const detail = document.getElementById('results-detail');
  detail.innerHTML = '';

  if (data.error && !data.results) {
    summary.className = 'results-summary fail';
    summary.textContent = data.timeout ? 'Timed out.' : data.error;
    if (data.stderr) detail.appendChild(consoleBlock('stderr', data.stderr, true));
    if (data.stdout) detail.appendChild(consoleBlock('stdout', data.stdout, false));
    return;
  }

  if (data.stage === 'compile') {
    summary.className = 'results-summary fail';
    summary.textContent = 'Compile error';
    detail.appendChild(consoleBlock('compiler', data.compileError || '', true));
    return;
  }

  const results = data.results || [];
  const passed = results.filter((r) => r.passed).length;
  const total = results.length;
  if (data.allPassed) {
    summary.className = 'results-summary pass';
    summary.textContent = `✓ All ${total} test${total === 1 ? '' : 's'} passed`;
  } else {
    summary.className = 'results-summary fail';
    summary.textContent = `✗ ${passed}/${total} tests passed`;
  }

  for (const r of results) {
    const div = document.createElement('div');
    div.className = 'case ' + (r.passed ? 'pass' : 'fail');
    let html = `<div class="case-head"><span class="verdict">${r.passed ? '✓ PASS' : '✗ FAIL'}</span>`;
    html += `<span class="case-name">${escapeHtml(r.name || '')}</span></div>`;
    if (r.input != null) html += `<div class="kv"><b>input:</b> ${escapeHtml(String(r.input))}</div>`;
    if (!r.passed) {
      html += `<div class="kv"><b>expected:</b> ${escapeHtml(String(r.expected))}</div>`;
      html += `<div class="kv"><b>got:</b> ${escapeHtml(String(r.got))}</div>`;
    }
    // Full traceback for a runtime error, kept with its own test.
    if (r.trace) {
      html += `<div class="trace"><span class="label">traceback</span>${escapeHtml(r.trace.replace(/\s+$/, ''))}</div>`;
    }
    // This test's own stdout, kept with its own test.
    if (r.stdout) {
      html += `<div class="case-stdout"><span class="label">stdout</span>${escapeHtml(r.stdout.replace(/\s+$/, ''))}</div>`;
    }
    div.innerHTML = html;
    detail.appendChild(div);
  }

  // Anything the program wrote outside a test (e.g. module-level prints) or to
  // stderr is shown once at the end.
  if (data.stdout) detail.appendChild(consoleBlock('stdout', data.stdout, false));
  if (data.stderr) detail.appendChild(consoleBlock('stderr', data.stderr, true));
}

function consoleBlock(label, text, isErr) {
  const div = document.createElement('div');
  div.className = 'console-block' + (isErr ? ' err' : '');
  div.innerHTML = `<span class="label">${escapeHtml(label)}</span>${escapeHtml(text)}`;
  return div;
}

function clearResults() {
  const summary = document.getElementById('results-summary');
  summary.className = 'results-summary';
  summary.textContent = 'Run your code to see results.';
  document.getElementById('results-detail').innerHTML = '';
}

// ---------------------------------------------------------------------------
// Navigation
// ---------------------------------------------------------------------------
function navigate(dir) {
  if (!current) return;
  const list = tabProblems();
  const idx = list.findIndex((p) => p.id === current.id);
  const next = list[idx + dir];
  if (next) openProblem(next.id);
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
function bumpStatus(id, status) {
  const p = problems.find((x) => x.id === id);
  if (!p) return;
  p.status = status;
  const dot = document.querySelector(`.prob-item[data-id="${id}"] .status-dot`);
  if (dot) dot.className = 'status-dot ' + status;
  renderOverall();
}

function softTabBackspace(cm) {
  if (cm.somethingSelected()) { cm.execCommand('delCharBefore'); return; }
  const cur = cm.getCursor();
  const before = cm.getLine(cur.line).slice(0, cur.ch);
  if (cur.ch > 0 && /^\s+$/.test(before)) {
    const unit = cm.getOption('indentUnit') || 4;
    let del = before.length % unit || unit;
    if (del > before.length) del = before.length;
    cm.replaceRange('', { line: cur.line, ch: cur.ch - del }, { line: cur.line, ch: cur.ch });
  } else {
    cm.execCommand('delCharBefore');
  }
}

function langLabel(l) {
  return { python: 'Python', c: 'C', cpp: 'C++' }[l] || l;
}

function escapeHtml(s) {
  return String(s)
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;');
}

// ---------------------------------------------------------------------------
// Sidebar collapse / show
// ---------------------------------------------------------------------------
(function () {
  const sidebar = document.getElementById('sidebar');
  const hideBtn = document.getElementById('sidebar-hide');
  const showBtn = document.getElementById('sidebar-show');

  function setCollapsed(val) {
    sidebar.classList.toggle('collapsed', val);
    localStorage.setItem('nlc-sidebar-collapsed', val ? '1' : '0');
  }

  hideBtn.addEventListener('click', () => setCollapsed(true));
  showBtn.addEventListener('click', () => setCollapsed(false));

  if (localStorage.getItem('nlc-sidebar-collapsed') === '1') {
    sidebar.classList.add('collapsed');
  }
}());

// ---------------------------------------------------------------------------
// Divider drag-resize (editor ↔ info pane)
// ---------------------------------------------------------------------------
(function () {
  const divider = document.getElementById('divider');
  const infoPane = document.getElementById('info-pane');
  const main = document.getElementById('main');

  divider.addEventListener('mousedown', (e) => {
    e.preventDefault();
    divider.classList.add('dragging');
    const startX = e.clientX;
    const startWidth = infoPane.getBoundingClientRect().width;

    function onMove(ev) {
      const delta = startX - ev.clientX;
      const mainW = main.getBoundingClientRect().width;
      const newWidth = Math.max(260, Math.min(mainW - 200, startWidth + delta));
      infoPane.style.width = newWidth + 'px';
    }

    function onUp() {
      divider.classList.remove('dragging');
      document.removeEventListener('mousemove', onMove);
      document.removeEventListener('mouseup', onUp);
      if (editor) editor.refresh();
    }

    document.addEventListener('mousemove', onMove);
    document.addEventListener('mouseup', onUp);
  });
}());
