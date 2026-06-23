#!/usr/bin/env node
'use strict';

// Set each problem's `languages` and `defaultLanguage` from the preferred
// language assigned in coding_practice_stack.md. Preserves every other field.
//
// - Phase 0/1 problems are authored in BOTH languages (default Python).
// - Every other problem is authored in its single preferred language.

const fs = require('fs');
const path = require('path');

const PROBLEMS_DIR = path.join(__dirname, '..', 'problems');

// Problems authored in both languages (default Python).
const BOTH = new Set([
  'two-sum', 'valid-anagram', 'reverse-string', 'best-time-to-buy-sell-stock',
  'valid-parentheses', 'two-sum-ii', 'valid-palindrome', 'container-with-most-water',
  'three-sum', 'maximum-subarray', 'longest-substring-without-repeating',
  'minimum-size-subarray-sum', 'product-of-array-except-self',
]);

// Preferred language per problem id (from the markdown's lang: tags).
const C_PROBLEMS = new Set([
  'design-circular-queue', 'ring-buffer',
  'reverse-linked-list', 'linked-list-cycle', 'merge-two-sorted-lists',
  'remove-nth-from-end', 'reorder-list',
  'number-of-1-bits', 'counting-bits', 'reverse-bits', 'single-number',
  'missing-number', 'sum-of-two-integers', 'bit-macros', 'swap-endianness',
  'power-of-two', 'bit-field-pack',
  'memcpy-memmove', 'fixed-point-multiply', 'debounce-fsm',
  'packet-framing-parser', 'crc8', 'cooperative-scheduler',
]);

function main() {
  let n = 0;
  for (const id of fs.readdirSync(PROBLEMS_DIR)) {
    if (id.startsWith('_')) continue;
    const metaPath = path.join(PROBLEMS_DIR, id, 'meta.json');
    if (!fs.existsSync(metaPath)) continue;
    const meta = JSON.parse(fs.readFileSync(metaPath, 'utf8'));

    if (BOTH.has(id)) {
      meta.languages = ['python', 'c'];
      meta.defaultLanguage = 'python';
    } else if (C_PROBLEMS.has(id)) {
      meta.languages = ['c'];
      meta.defaultLanguage = 'c';
    } else {
      meta.languages = ['python'];
      meta.defaultLanguage = 'python';
    }

    fs.writeFileSync(metaPath, JSON.stringify(meta, null, 2) + '\n');
    n++;
  }
  console.log(`Updated languages/defaultLanguage on ${n} problems.`);
}

main();
