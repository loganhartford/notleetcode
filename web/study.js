'use strict';

let studyPlan, reviews = {}, selectedDay = 'sun';
const labels = { stuck: 'Stuck', assisted: 'Used help', cold: 'Independent', retained: 'Retained', untested: 'Not checked' };
const escape = (value) => String(value).replace(/[&<>"']/g, (c) => ({ '&':'&amp;', '<':'&lt;', '>':'&gt;', '"':'&quot;', "'":'&#39;' }[c]));

// Support inline code without allowing HTML from the plan into the page.
function inlineCode(value) {
  return String(value).split(/(`[^`\n]+`)/g).map((part) =>
    part.startsWith('`') && part.endsWith('`')
      ? `<code>${escape(part.slice(1, -1))}</code>` : escape(part)
  ).join('');
}

function oralQuestionsHTML(item) {
  return `<p class="oral-instructions">${escape(item.instructions)}</p>
    <ol class="oral-questions">${item.questions.map((question) => `<li>
      <h4>${escape(question.title)}</h4>
      <p class="question-prompt">${inlineCode(question.prompt)}</p>
      ${question.code ? `<pre class="question-code"><code>${escape(question.code)}</code></pre>` : ''}
      <details class="question-answer"><summary>Reveal answer<span class="sr-only">: ${escape(question.title)}</span></summary>
        <ul>${question.answer.map((point) => `<li>${inlineCode(point)}</li>`).join('')}</ul>
      </details>
    </li>`).join('')}</ol>`;
}

function localDate(iso = new Date().toISOString()) {
  const parts = new Intl.DateTimeFormat('en-US', { timeZone: studyPlan.timezone, year:'numeric', month:'2-digit', day:'2-digit' }).formatToParts(new Date(iso));
  const get = (type) => parts.find((p) => p.type === type).value;
  return `${get('year')}-${get('month')}-${get('day')}`;
}

function nextDate(date) {
  const next = new Date(date + 'T12:00:00Z');
  next.setUTCDate(next.getUTCDate() + 1);
  return next.toISOString().slice(0, 10);
}

function recallState(id) {
  const history = reviews[id] || [];
  const last = history.at(-1);
  if (!last) return { status:'untested', due:false, next:'' };
  if (last.rating !== 'cold') return { status:last.rating, due:true, next:'Retry today' };
  const dates = new Set();
  for (let i = history.length - 1; i >= 0 && history[i].rating === 'cold'; --i) dates.add(localDate(history[i].at));
  const next = nextDate(localDate(last.at));
  return { status:dates.size > 1 ? 'retained' : 'cold', due:next <= localDate(), next:`Recall ${next}` };
}

function renderStats() {
  const core = studyPlan.items.filter((i) => i.tier === 'core');
  const checked = core.filter((i) => recallState(i.id).status !== 'untested').length;
  const retained = core.filter((i) => recallState(i.id).status === 'retained').length;
  const due = core.filter((i) => recallState(i.id).due).length;
  document.getElementById('stats').innerHTML = [
    [core.filter((i) => i.kind === 'code').length, 'core coding exercises'], [core.filter((i) => i.kind === 'oral').length, 'oral / mock drills'], [`${checked}/${core.length}`, 'freshly assessed'], [retained, 'retained across days'], [due, 'core items due']
  ].map(([value, label]) => `<div class="stat"><strong>${value}</strong><span>${label}</span></div>`).join('');
}

function renderSession() {
  document.getElementById('days').innerHTML = studyPlan.sessions.map((s) => `<button data-day="${s.id}" aria-pressed="${s.id === selectedDay}">${escape(s.label)}</button>`).join('');
  document.querySelectorAll('[data-day]').forEach((button) => button.onclick = () => {
    selectedDay = button.dataset.day;
    document.getElementById('filter').value = 'session';
    renderSession(); renderCards();
  });
  const session = studyPlan.sessions.find((s) => s.id === selectedDay);
  document.getElementById('session').innerHTML = `<div class="session-panel">
    <div class="session-head"><h3>${escape(session.title)}</h3><span class="session-time">${session.minutes} focused minutes</span></div>
    <p class="muted">${escape(session.goal)}</p>
    <div class="blocks">${session.blocks.map((block) => `<div class="block"><span class="block-time">${block.minutes} min</span><div><strong>${escape(block.title)}</strong><p>${escape(block.detail)}</p><div class="chips">${block.items.map((id) => `<button class="chip" data-show="${escape(id)}">${escape(studyPlan.items.find((i) => i.id === id).title)}</button>`).join('')}</div></div></div>`).join('')}</div>
    <p class="short-route">${escape(session.shortRoute)}</p></div>`;
  document.querySelectorAll('[data-show]').forEach((b) => b.onclick = () => showItem(b.dataset.show));
}

function showItem(id) {
  document.getElementById('filter').value = 'all';
  renderCards();
  location.hash = id;
  document.getElementById(id)?.scrollIntoView({ behavior:'smooth', block:'start' });
}

function cardHTML(item) {
  const state = recallState(item.id);
  const history = reviews[item.id] || [];
  return `<article class="card${item.questions ? ' oral-card' : ''}" id="${escape(item.id)}">
    <div class="card-top"><span>${item.kind === 'code' ? 'C PRACTICE' : 'SPEAK ALOUD'} · ${item.minutes} MIN ${item.new ? '<span class="new">· NEW</span>' : ''}</span><span class="badge ${state.status}">${labels[state.status]}</span></div>
    <h3>${escape(item.title)}</h3><p>${escape(item.why)}</p>
    ${item.questions ? oralQuestionsHTML(item) : `<ul>${item.checks.map((c) => `<li>${inlineCode(c)}</li>`).join('')}</ul>`}
    ${item.variation ? `<details><summary>Transfer question</summary><p>${escape(item.variation)}</p></details>` : ''}
    ${item.kind === 'code' ? `<div class="actions"><a class="primary" href="/#${escape(item.id)}" target="_blank" rel="noopener">Open question ↗</a></div>` : ''}
    <form class="review-form" data-id="${escape(item.id)}">
      <div class="form-row"><label>How did this attempt go?<select name="rating" required><option value="">Choose honestly…</option><option value="stuck">Stuck</option><option value="assisted">Used help</option><option value="cold">Independent</option></select></label><label>Minutes<input name="minutes" type="number" min="0" max="600" value="${item.minutes}" required></label></div>
      <label for="note-${escape(item.id)}">One mistake → one prevention rule</label><textarea id="note-${escape(item.id)}" name="note" maxlength="2000" placeholder="e.g. Forgot the terminator → reserve one byte before copying."></textarea>
      <div class="save-row"><button class="record" type="submit">Log attempt</button>${history.length ? '<button class="undo" type="button">Undo last log</button>' : ''}<span class="feedback" role="status"></span></div>
    </form>
    <div class="history">${state.next ? escape(state.next) : 'No fresh assessment yet.'}${history.length ? `<details><summary>${history.length} logged attempt${history.length === 1 ? '' : 's'}</summary><ol>${history.slice().reverse().map((r) => `<li>${escape(localDate(r.at))} · ${labels[r.rating]} · ${r.minutes}m${r.note ? ' — ' + escape(r.note) : ''}</li>`).join('')}</ol></details>` : ''}</div>
  </article>`;
}

function renderCards() {
  const filter = document.getElementById('filter').value;
  const session = studyPlan.sessions.find((s) => s.id === selectedDay);
  const sessionIds = new Set(session.blocks.flatMap((b) => b.items));
  const items = studyPlan.items.filter((item) => filter === 'all' ||
    (filter === 'session' && sessionIds.has(item.id)) ||
    (filter === 'core' && item.tier === 'core') ||
    (filter === 'extension' && item.tier === 'extension') ||
    (filter === 'due' && recallState(item.id).due));
  document.getElementById('cards').innerHTML = items.map(cardHTML).join('') || '<p class="empty">Nothing due yet. Start with this session and log an honest recall rating.</p>';
  wireCards();
}

function wireCards() {
  document.querySelectorAll('.review-form').forEach((form) => {
    form.onsubmit = (event) => { event.preventDefault(); saveReview(form, 'record'); };
    const undo = form.querySelector('.undo');
    if (undo) undo.onclick = () => saveReview(form, 'undo');
  });
}

async function saveReview(form, action) {
  const feedback = form.querySelector('.feedback');
  const id = form.dataset.id;
  const buttons = form.querySelectorAll('button');
  buttons.forEach((b) => b.disabled = true);
  feedback.textContent = 'Saving…'; feedback.classList.remove('failed');
  try {
    const response = await fetch('/api/study/review', {
      method:'POST', headers:{ 'Content-Type':'application/json' },
      body:JSON.stringify({ id, action, rating:form.elements.rating.value, minutes:Number(form.elements.minutes.value), note:form.elements.note.value.trim() })
    });
    const data = await response.json();
    if (!response.ok) throw new Error(data.error || 'Could not save');
    reviews = data.reviews;
    // Replace only this card so unsaved notes in other cards survive a log.
    const card = document.getElementById(id);
    card.outerHTML = cardHTML(studyPlan.items.find((i) => i.id === id));
    wireCards(); renderStats();
    document.getElementById(id).querySelector('.feedback').textContent = action === 'undo' ? 'Last log removed' : 'Attempt saved';
  } catch (error) {
    feedback.textContent = error.message; feedback.classList.add('failed');
  } finally {
    buttons.forEach((b) => b.disabled = false);
  }
}

async function initStudy() {
  try {
    const response = await fetch('/api/study');
    if (!response.ok) throw new Error('Could not load the study plan. Check that the updated notleetcode server is running.');
    const data = await response.json();
    studyPlan = data.plan; reviews = data.reviews;
    document.getElementById('identity').textContent = `${data.user} · Thursday prep`;
    for (const key of ['intro', 'scope', 'strategy', 'defer']) document.getElementById(key).textContent = studyPlan[key];
    document.getElementById('rules').innerHTML = studyPlan.rules.map((r) => `<li>${escape(r)}</li>`).join('');
    document.getElementById('cautions').innerHTML = studyPlan.cautions.map((r) => `<li>${escape(r)}</li>`).join('');
    document.getElementById('sources').innerHTML = studyPlan.sources.map((s) => `<a href="${escape(s.url)}" target="_blank" rel="noopener" title="${escape(s.note)}">${escape(s.title)} ↗</a>`).join('');
    const dayIndex = Math.max(0, Math.min(4, Math.floor((Date.parse(localDate() + 'T12:00:00Z') - Date.parse('2026-10-25T12:00:00Z')) / 86400000)));
    selectedDay = studyPlan.sessions[dayIndex].id;
    document.getElementById('filter').onchange = renderCards;
    renderStats(); renderSession(); renderCards();
    if (studyPlan.items.some((item) => item.id === location.hash.slice(1))) showItem(location.hash.slice(1));
  } catch (error) {
    const host = document.getElementById('error');
    host.hidden = false; host.textContent = error.message;
  }
}

initStudy();
