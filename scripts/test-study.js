#!/usr/bin/env node
'use strict';

// Isolated API/retention regression checks. Never writes real user progress.
const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const { spawn } = require('node:child_process');
const vm = require('node:vm');
const root = path.join(__dirname, '..');
const temp = fs.mkdtempSync(path.join(os.tmpdir(), 'nlc-study-test-'));
let server;

async function main() {
  const plan = JSON.parse(fs.readFileSync(path.join(root, 'web/study-plan.json')));
  const ids = plan.items.map((i) => i.id);
  assert.equal(new Set(ids).size, ids.length);
  for (const session of plan.sessions) {
    assert.equal(session.blocks.reduce((n, b) => n + b.minutes, 0), session.minutes);
    for (const id of session.blocks.flatMap((b) => b.items)) assert(ids.includes(id));
  }
  for (const item of plan.items.filter((i) => i.kind === 'code')) {
    const meta = JSON.parse(fs.readFileSync(path.join(root, 'problems', item.id, 'meta.json')));
    assert.equal(meta.tab, 'firmware');
    assert(meta.languages.includes('c'));
  }
  const sandbox = vm.createContext({ Intl, Date, Set });
  const source = fs.readFileSync(path.join(root, 'web/study.js'), 'utf8').replace(/initStudy\(\);\s*$/, '');
  vm.runInContext(source, sandbox);
  vm.runInContext(`studyPlan = ${JSON.stringify(plan)}; reviews = {};`, sandbox);
  assert.equal(vm.runInContext("recallState('fw-strlen').status", sandbox), 'untested');
  vm.runInContext(`reviews['fw-strlen'] = [{rating:'cold',at:'2026-10-25T18:00:00Z'}, {rating:'cold',at:'2026-10-25T20:00:00Z'}];`, sandbox);
  assert.equal(vm.runInContext("recallState('fw-strlen').status", sandbox), 'cold');
  vm.runInContext(`reviews['fw-strlen'].push({rating:'cold',at:'2026-10-26T18:00:00Z'});`, sandbox);
  assert.equal(vm.runInContext("recallState('fw-strlen').status", sandbox), 'retained');
  vm.runInContext(`reviews['fw-strlen'].push({rating:'assisted',at:'2026-10-27T18:00:00Z'});`, sandbox);
  assert.equal(vm.runInContext("recallState('fw-strlen').status", sandbox), 'assisted');
  assert.equal(vm.runInContext("recallState('fw-strlen').due", sandbox), true);
  assert.equal(vm.runInContext("localDate('2026-10-26T02:00:00Z')", sandbox), '2026-10-25');

  // Discover the ephemeral port in this fixture's startup message.
  const serverSource = fs.readFileSync(path.join(root, 'server.js'), 'utf8')
    .replace('http://localhost:${PORT}  (user:', 'http://localhost:${server.address().port}  (user:');
  fs.writeFileSync(path.join(temp, 'server.js'), serverSource);
  fs.mkdirSync(path.join(temp, 'web'));
  fs.copyFileSync(path.join(root, 'web/study-plan.json'), path.join(temp, 'web/study-plan.json'));
  fs.mkdirSync(path.join(temp, 'problems'));
  for (const id of ['_lib', 'fw-strlen']) fs.cpSync(path.join(root, 'problems', id), path.join(temp, 'problems', id), { recursive:true });
  fs.mkdirSync(path.join(temp, 'progress'));
  fs.mkdirSync(path.join(temp, 'solutions/tester'), { recursive:true });
  const originalCode = '#include <stddef.h>\nsize_t c_strlen(const char *s) { size_t n=0; while(s[n]) ++n; return n; }\n';
  const oldProgress = '{"fw-strlen":{"status":"solved","flagged":true}}\n';
  fs.writeFileSync(path.join(temp, 'solutions/tester/fw-strlen.c'), originalCode);
  fs.writeFileSync(path.join(temp, 'progress/tester.json'), oldProgress);

  server = spawn(process.execPath, ['server.js'], { cwd:temp, env:{ ...process.env, NLC_USER:'tester', PORT:'0' }, stdio:['ignore','pipe','pipe'] });
  // PORT=0 binds an available port; use a tiny test-only adaptation for discovery.
  // This fixture has its own server.js and cannot affect the running application.
  const address = await new Promise((resolve, reject) => {
    let log = '';
    const timeout = setTimeout(() => reject(new Error('Server startup timed out')), 5000);
    server.stdout.on('data', (data) => {
      log += data;
      const match = log.match(/http:\/\/localhost:(\d+)/);
      if (match) { clearTimeout(timeout); resolve(match[1]); }
    });
    server.once('exit', () => { clearTimeout(timeout); reject(new Error('Server exited during startup')); });
  });
  const base = `http://localhost:${address}`;
  const request = async (route, body) => {
    const response = await fetch(base + route, body === undefined ? {} : { method:'POST', headers:{'Content-Type':'application/json'}, body:JSON.stringify(body) });
    return { status:response.status, data:await response.json() };
  };
  let response = await request('/api/study');
  assert.equal(response.status, 200); assert.deepEqual(response.data.reviews, {});
  response = await request('/api/study/review', { id:'fw-strlen', action:'record', rating:'assisted', minutes:12, note:'Forgot terminator' });
  assert.equal(response.status, 200); assert.equal(response.data.reviews['fw-strlen'][0].rating, 'assisted');
  assert.equal((await request('/api/study')).data.reviews['fw-strlen'][0].note, 'Forgot terminator');
  for (const patch of [{ id:'../escape' }, { rating:'solved' }, { minutes:-1 }, { minutes:1.5 }, { note:'x'.repeat(2001) }]) {
    assert.equal((await request('/api/study/review', { id:'fw-strlen', action:'record', rating:'cold', minutes:10, note:'', ...patch })).status, 400);
  }
  response = await request('/api/study/review', { id:'fw-strlen', action:'undo' });
  assert.deepEqual(response.data.reviews['fw-strlen'], []);
  const savedPath = path.join(temp, 'solutions/tester/fw-strlen.c');
  const readProgress = () => JSON.parse(fs.readFileSync(path.join(temp, 'progress/tester.json')))['fw-strlen'];
  const failingCode = '#include <stddef.h>\nsize_t c_strlen(const char *s) {return 0;}';
  // Recovery must never replace newer project work or downgrade a matching pass.
  const originalProgress = fs.readFileSync(path.join(temp, 'progress/tester.json'), 'utf8');
  response = await request('/api/save', { id:'fw-strlen', language:'c', code:failingCode, recover:true });
  assert.equal(response.status, 409);
  assert.equal(fs.readFileSync(savedPath, 'utf8'), originalCode);
  assert.equal(fs.readFileSync(path.join(temp, 'progress/tester.json'), 'utf8'), originalProgress);
  response = await request('/api/save', { id:'fw-strlen', language:'c', code:originalCode, recover:true });
  assert.equal(response.status, 200);
  assert.equal(fs.readFileSync(savedPath, 'utf8'), originalCode);
  assert.equal(fs.readFileSync(path.join(temp, 'progress/tester.json'), 'utf8'), originalProgress);
  fs.unlinkSync(savedPath);
  response = await request('/api/save', { id:'fw-strlen', language:'c', code:failingCode, recover:true });
  assert.equal(response.status, 200);
  assert.equal(fs.readFileSync(savedPath, 'utf8'), failingCode);
  assert.equal(readProgress().status, 'attempted');
  response = await request('/api/save', { id:'fw-strlen', language:'c', code:failingCode });
  assert.equal(response.status, 200);
  assert.equal(fs.readFileSync(savedPath, 'utf8'), failingCode);
  assert.equal((await request('/api/problem/fw-strlen')).data.saved.c, failingCode);
  assert.equal(readProgress().status, 'attempted');
  assert.equal(readProgress().flagged, true);
  response = await request('/api/run', { id:'fw-strlen', language:'c', code:originalCode });
  assert.equal(response.status, 200); assert.equal(response.data.allPassed, true);
  assert.equal(fs.readFileSync(savedPath, 'utf8'), originalCode);
  assert.equal(readProgress().status, 'solved');
  response = await request('/api/run', { id:'fw-strlen', language:'c', code:failingCode });
  assert.equal(response.data.allPassed, false);
  assert.equal(response.data.status, 'attempted');
  assert.equal(fs.readFileSync(savedPath, 'utf8'), failingCode);
  assert.equal(readProgress().status, 'attempted');
  // Legacy tabs now use the same persistent latest-attempt workflow.
  response = await request('/api/practice/run', { id:'fw-strlen', language:'c', code:originalCode });
  assert.equal(response.data.allPassed, true);
  assert.equal(fs.readFileSync(savedPath, 'utf8'), originalCode);
  for (const route of ['/api/save', '/api/run', '/api/practice/run']) {
    for (const patch of [{ id:'../bad' }, { id:'missing' }, { language:'python' }, { code:null }]) {
      assert.equal((await request(route, { id:'fw-strlen', language:'c', code:'', ...patch })).status, 400);
      assert.equal(fs.readFileSync(savedPath, 'utf8'), originalCode);
    }
  }
  // An empty edit is legitimate latest work, not a reason to restore old code.
  assert.equal((await request('/api/save', { id:'fw-strlen', language:'c', code:'' })).status, 200);
  assert.equal((await request('/api/problem/fw-strlen')).data.saved.c, '');
  console.log('PASS: plan references/time budgets, recall across days, review persistence/validation/undo, latest code persistence, current test status, safe browser recovery, and legacy-route compatibility.');
}

main().catch((error) => { console.error(error); process.exitCode = 1; }).finally(async () => {
  if (server && server.exitCode === null) {
    const stopped = new Promise((resolve) => server.once('exit', resolve));
    server.kill(); await stopped;
  }
  fs.rmSync(temp, { recursive:true, force:true });
});
