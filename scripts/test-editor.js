#!/usr/bin/env node
'use strict';

// Run the browser editor logic with a minimal DOM and controllable network.
// No real browser storage, user solutions, or progress files are touched.
const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const vm = require('node:vm');

function fixture() {
  const elements = new Map();
  const storage = new Map();
  const document = { getElementById(id) {
    if (!elements.has(id)) elements.set(id, { textContent:'', value:'', addEventListener(){}, style:{}, classList:{ add(){}, remove(){}, toggle(){} } });
    return elements.get(id);
  } };
  const localStorage = {
    get length() { return storage.size; },
    key(i) { return [...storage.keys()][i]; },
    getItem(key) { return storage.get(key) ?? null; },
    setItem(key, value) { storage.set(key, value); },
    removeItem(key) { storage.delete(key); },
  };
  const context = vm.createContext({ document, localStorage, window:{ addEventListener(){} }, setTimeout, clearTimeout });
  vm.runInContext(fs.readFileSync(path.join(__dirname, '../web/app.js'), 'utf8'), context);
  vm.runInContext(`
    editor = { value:'old C', getValue(){return this.value;}, setValue(v){this.value=v;}, setOption(){} };
    current = { id:'fw-strlen', saved:{ c:'old C', python:'old Python' }, starter:{c:'starter'}, progress:{status:'solved'} };
    currentLang = 'c'; activeUser = 'tester';
    renderStatusBadge = () => {}; bumpStatus = () => {};
    setupSolutionBlock = () => {}; clearResults = () => {}; loadProblems = async () => {};
  `, context);
  return { context, storage, elements, run: (code) => vm.runInContext(code, context) };
}

async function main() {
  let f = fixture();
  f.context.fetch = async () => ({ ok:false });
  f.run("editor.value = 'unsaved C'");
  assert.equal(await f.run('saveNow()'), false);
  assert.equal(f.run('current.saved.c'), 'old C');
  assert.match(f.elements.get('save-state').textContent, /Not saved/);
  await f.run("switchLang('python')");
  assert.equal(f.run('currentLang'), 'c');
  assert.equal(f.run('editor.value'), 'unsaved C');

  f.context.fetch = async () => ({ ok:true });
  await f.run("switchLang('python')");
  assert.equal(f.run('current.saved.c'), 'unsaved C');
  assert.equal(f.run('editor.value'), 'old Python');
  await f.run("switchLang('c')");
  assert.equal(f.run('editor.value'), 'unsaved C');

  f = fixture();
  const writes = [], releases = [];
  f.context.fetch = async (_route, options) => {
    writes.push(JSON.parse(options.body).code);
    await new Promise((resolve) => releases.push(resolve));
    return { ok:true };
  };
  const first = f.run("editor.value = 'edit one'; saveNow()");
  await new Promise(setImmediate);
  const second = f.run("editor.value = 'edit two'; saveNow()");
  await new Promise(setImmediate);
  assert.deepEqual(writes, ['edit one']);
  releases.shift()(); await first;
  await new Promise(setImmediate);
  assert.deepEqual(writes, ['edit one', 'edit two']);
  releases.shift()(); await second;
  assert.equal(f.run('current.saved.c'), 'edit two');

  f = fixture();
  f.run("problems = [{id:'fw-strlen', languages:['c']}, {id:'fw-bit-ops', languages:['c']}]");
  f.storage.set('nlc-practice:tester:100:fw-strlen:c', 'old draft');
  f.storage.set('nlc-practice:tester:200:fw-strlen:c', 'latest draft');
  f.storage.set('nlc-practice:tester:300:fw-bit-ops:c', 'failed save draft');
  f.storage.set('nlc-practice:someoneelse:400:fw-strlen:c', 'other user');
  const recovered = [];
  f.context.fetch = async (_route, options) => {
    const body = JSON.parse(options.body);
    recovered.push(body);
    return { ok:body.id !== 'fw-bit-ops' };
  };
  await f.run('recoverBrowserAttempts()');
  assert.equal(recovered[0].code, 'latest draft');
  assert.equal(recovered.length, 2);
  assert.equal(f.storage.has('nlc-practice:tester:100:fw-strlen:c'), false);
  assert.equal(f.storage.has('nlc-practice:tester:200:fw-strlen:c'), false);
  assert.equal(f.storage.get('nlc-practice:tester:300:fw-bit-ops:c'), 'failed save draft');
  assert.equal(f.storage.get('nlc-practice:someoneelse:400:fw-strlen:c'), 'other user');

  f = fixture();
  f.run("problems = [{id:'fw-strlen', languages:['c']}, {id:'fw-bit-ops', languages:['c']}]");
  f.storage.set('nlc-practice:tester:100:fw-strlen:c', 'conflicting old draft');
  f.storage.set('nlc-practice:tester:200:fw-bit-ops:c', 'recoverable draft');
  const migrationRequests = [];
  f.context.fetch = async (_route, options) => {
    const body = JSON.parse(options.body);
    migrationRequests.push(body);
    return body.id === 'fw-strlen' ? { ok:false, status:409 } : { ok:true, status:200 };
  };
  await f.run('recoverBrowserAttempts()');
  assert.equal(migrationRequests.length, 2);
  assert(migrationRequests.every((body) => body.recover === true));
  assert.equal(f.storage.get('nlc-practice:tester:100:fw-strlen:c'), 'conflicting old draft');
  assert.equal(f.storage.has('nlc-practice:tester:200:fw-bit-ops:c'), false);
  const message = f.elements.get('recovery-message').textContent;
  assert.match(message, /Recovered 1 browser attempt/);
  assert.match(message, /Existing project code kept/);
  assert.match(message, /drafts retained/);
  console.log('PASS: honest save failures, safe language switching, ordered latest edits, and recoverable user-scoped browser draft migration.');
}

main().catch((error) => { console.error(error); process.exitCode = 1; });
