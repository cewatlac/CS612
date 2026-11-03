(() => {
  'use strict';
  const lectures=window.COURSE_DATA||[];
  const grid=document.getElementById('lecture-grid');
  const input=document.getElementById('lecture-search');
  const results=document.getElementById('results-context');
  const count=document.getElementById('result-count');
  const empty=document.getElementById('empty-results');
  const buttons=[...document.querySelectorAll('.filter-button')];
  const esc=v=>String(v??'').replace(/[&<>"']/g,ch=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[ch]));
  let group='All';
  const label=kind=>({CPP:'C++',PDF:'PDF',MD:'MD',HPP:'C++',TXT:'TXT'}[kind]||kind);
  const preview=(f)=>f.kind==='PDF'?encodeURI(f.url):`viewer.html?file=${encodeURIComponent(f.url)}`;
  function fileRow(f){
    const url=encodeURI(f.url), view=preview(f);
    return `<li class="file-row"><span class="file-icon file-${esc(f.kind.toLowerCase())}">${esc(label(f.kind))}</span><span class="file-name-wrap"><a class="file-name" href="${view}">${esc(f.name)}</a>${f.description?`<span class="file-description">${esc(f.description)}</span>`:''}</span><span class="file-links"><a href="${view}" aria-label="Open ${esc(f.name)}">Open ↗</a><a href="${url}" download aria-label="Download ${esc(f.name)}">↓</a></span></li>`;
  }
  function createCard(l){
    return `<article id="lecture-${String(l.num).padStart(2,'0')}" class="lecture-card" data-number="${l.num}">
      <div class="card-intro"><div class="card-number">LECTURE <strong>${String(l.num).padStart(2,'0')}</strong></div><span class="card-category">${esc(l.tag)}</span></div>
      <h3>${esc(l.title)}</h3><p class="card-summary">${esc(l.summary)}</p>
      <div class="card-meta"><span><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8" aria-hidden="true"><rect x="3" y="4" width="18" height="17" rx="2"/><path d="M7 2v4M17 2v4M3 9h18"/></svg>${esc(l.dateLabel)}</span><span>${l.slides} slides</span><span>${l.codeCount} C++ files</span></div>
      <a class="open-lecture" href="${encodeURI(l.html)}"><span>Open interactive slides</span><span class="open-arrow" aria-hidden="true">↗</span></a>
      <div class="quick-links"><a href="${encodeURI(l.slidesPdf)}" aria-label="Open Lecture ${l.num} slides as PDF"><span class="quick-icon">▤</span> Slides PDF</a><a href="${encodeURI(l.exercisePdf)}" aria-label="Open Lecture ${l.num} in-class exercises PDF"><span class="quick-icon">✎</span> Exercises PDF</a></div>
      <details class="resources-panel"><summary><span>All lecture files <b>${l.files.length}</b></span><span class="accordion-symbol" aria-hidden="true">+</span></summary><div class="resources-inner"><ul class="file-list">${l.files.map(fileRow).join('')}</ul><a class="download-pack" href="${encodeURI(l.zip)}" download>Download Lecture ${String(l.num).padStart(2,'0')} package (ZIP) <span>↓</span></a></div></details>
    </article>`;
  }
  function render(){
    const q=input.value.toLocaleLowerCase().trim();
    const shown=lectures.filter(l=>(group==='All'||l.group===group)&&(!q||[String(l.num),`lecture ${l.num}`,l.title,l.summary,l.tag,l.group,l.keywords,...l.files.map(f=>f.name)].join(' ').toLocaleLowerCase().includes(q)));
    grid.innerHTML=shown.map(createCard).join('');
    count.textContent=String(shown.length);
    empty.hidden=shown.length!==0;
    results.textContent=shown.length===lectures.length&&!q&&group==='All'?`Showing all ${lectures.length} lectures in course order`:`Showing ${shown.length} of ${lectures.length} lectures${q?` matching “${input.value.trim()}”`:''}`;
  }
  buttons.forEach(btn=>btn.addEventListener('click',()=>{group=btn.dataset.group;buttons.forEach(b=>{const active=b===btn;b.classList.toggle('is-active',active);b.setAttribute('aria-pressed',String(active));});render();}));
  input.addEventListener('input',render);
  document.getElementById('reset-search').addEventListener('click',()=>{input.value='';buttons[0].click();input.focus();});
  document.addEventListener('keydown',e=>{if(e.key==='/'&&!['INPUT','TEXTAREA'].includes(document.activeElement.tagName)&&!e.ctrlKey&&!e.metaKey){e.preventDefault();input.focus();}});
  render();
  const hash=location.hash.match(/^#lecture-(\d{2})$/);
  if(hash){const target=document.getElementById('lecture-'+hash[1]);if(target){target.scrollIntoView();const detail=target.querySelector('details');if(detail)detail.open=true;}}
})();
