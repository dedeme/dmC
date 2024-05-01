// Copyright 03-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';

class Domo {
  constructor (e) {
    this.el = e;
  }
  // \<domo> -> <domo>
  add (o) { this.el.appendChild(o.e); return this; }
  // \[<domo>] -> <domo>
  adds (a) { for (const o of a) this.el.appendChild(o.e); return this; }
  // \s, * -> <domo>
  att (k, v) { this.el.setAttribute(k, v); return this; }
  // \b -> <domo>
  checked (v) { this.el.checked = v; return this; }
  // \b -> <domo>
  disabled (v) { this.el.disabled = v; return this; }
  // \s -> *
  getAtt (k) { return this.el.getAttribute(k) }
  // \-> s
  getClass () { return this.el.className }
  // \-> s
  getHtml () { return this.el.innerHTML }
  // \-> s
  getStyle () { return this.el.getAttribute("style") }
  // \-> s
  getText () { return this.el.textContent }
  // \-> *
  getValue () { return this.el.value }
  // DOM element.
  // *
  get e() { return this.el; }
  // \s -> <domo>
  html (s) { this.el.innerHTML = s; return this; }
  // \-> b
  isChecked () { return this.el.checked }
  // \-> b
  isDisabled () { return this.el.disabled }
  // \s -> <domo>
  klass (s) { this.el.className = s; return this; }
  // Type is one of:
  // "blur"; "change"; "click"; "dblclick"; "focus"; "input";
  // "keydown"; "keypress"; "keyup"; "load";
  // "mousedown"; "mousemove"; "mouseout"; "mouseover"; "mouseup"; "wheel";
  // "select"; "selectstart"; "submit"
  // \s, (\->() | \<event>->()) -> <domo>
  on (type, fn) { this.el.addEventListener(type, fn, false); return this; }
  // <domo> -> <domo>
  remove (o) { this.el.removeChild(o.e); return this; }
  // \-> <domo>
  removeAll () { this.el.innerHTML = ""; return this; }
  // \s, s-> <domo>
  setStyle (k, v) { this.el.style.setProperty(k, v); return this; }
  // \s-> <domo>
  style (v) { this.el.setAttribute("style", v); return this; }
  // \s -> <domo>
  text (s) { this.el.textContent = s; return this; }
  // \* -> <domo>
  value (v) { this.el.value = v; return this; }
}

// \* -> <domo>
export function mk (e) {
  return new Domo(e);
}
