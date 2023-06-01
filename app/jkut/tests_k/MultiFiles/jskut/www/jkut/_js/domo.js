// Copyright 03-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';


// \* -> <domo>
export function mk (e) {
  const d = {
    // \<domo> -> <domo>
    add: (o) => { e.appendChild(o.e); return d; },
    // \[<domo>] -> <domo>
    adds: (a) => { a.forEach(o => e.appendChild(o.e)); return d; },
    // \s, * -> <domo>
    att: (k, v) => { e.setAttribute(k, v); return d; },
    // \b -> <domo>
    checked: (v) => { e.checked = v; return d; },
    // \b -> <domo>
    disabled: (v) => { e.disabled = v; return d; },
    // \s -> *
    getAtt: (k) => e.getAttribute(k),
    // \-> s
    getClass: () => e.className,
    // \-> s
    getHtml: () => e.innerHTML,
    // \-> s
    getStyle: () => e.getAttribute("style"),
    // \-> s
    getText: () => e.textContent,
    // \-> *
    getValue: () => e.value,
    // DOM element.
    // *
    e: e,
    // \s -> <domo>
    html: (s) => { e.innerHTML = s; return d; },
    // \-> b
    isChecked: () => e.checked,
    // \-> b
    isDisabled: () => e.disabled,
    // \s -> <domo>
    klass: (s) => { e.className = s; return d; },
    // Type is one of:
    // "blur"; "change"; "click"; "dblclick"; "focus"; "input";
    // "keydown"; "keypress"; "keyup"; "load";
    // "mousedown"; "mousemove"; "mouseout"; "mouseover"; "mouseup"; "wheel";
    // "select"; "selectstart"; "submit"
    // \s, (\->() | \<event>->()) -> <domo>
    on: (type, fn) => { e.addEventListener(type, fn, false); return d; },
    // <domo> -> <domo>
    remove: (o) => { e.removeChild(o.e); return d; },
    // \-> <domo>
    removeAll: () => { e.innerHTML = ""; return d; },
    // \s, s-> <domo>
    setStyle: (k, v) => { e.style.setProperty(k, v); return d; },
    // \s-> <domo>
    style: (v) => { e.setAttribute("style", v); return d; },
    // \s -> <domo>
    text: (s) => { e.textContent = s; return d; },
    // \* -> <domo>
    value: (v) => { e.value = v; return d; }
  };
  return d;
}

