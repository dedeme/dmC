// Copyright 06-Jul-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

char *domox_bget (void) {return
  "// Copyright 03-May-2023 ºDeme\n"
  "// GNU General Public License - V3 <http://www.gnu.org/licenses/>\n"
  "\n"
  "import * as sys from './sys.js';\n"
  "\n"
  "\n"
  "// \\* -> <domo>\n"
  "export function mk (e) {\n"
  "  const d = {\n"
  "    // \\<domo> -> <domo>\n"
  "    add: (o) => { e.appendChild(o.e); return d; },\n"
  "    // \\[<domo>] -> <domo>\n"
  "    adds: (a) => { a.forEach(o => e.appendChild(o.e)); return d; },\n"
  "    // \\s, * -> <domo>\n"
  "    att: (k, v) => { e.setAttribute(k, v); return d; },\n"
  "    // \\b -> <domo>\n"
  "    checked: (v) => { e.checked = v; return d; },\n"
  "    // \\b -> <domo>\n"
  "    disabled: (v) => { e.disabled = v; return d; },\n"
  "    // \\s -> *\n"
  "    getAtt: (k) => e.getAttribute(k),\n"
  "    // \\-> s\n"
  "    getClass: () => e.className,\n"
  "    // \\-> s\n"
  "    getHtml: () => e.innerHTML,\n"
  "    // \\-> s\n"
  "    getStyle: () => e.getAttribute(\"style\"),\n"
  "    // \\-> s\n"
  "    getText: () => e.textContent,\n"
  "    // \\-> *\n"
  "    getValue: () => e.value,\n"
  "    // DOM element.\n"
  "    // *\n"
  "    e: e,\n"
  "    // \\s -> <domo>\n"
  "    html: (s) => { e.innerHTML = s; return d; },\n"
  "    // \\-> b\n"
  "    isChecked: () => e.checked,\n"
  "    // \\-> b\n"
  "    isDisabled: () => e.disabled,\n"
  "    // \\s -> <domo>\n"
  "    klass: (s) => { e.className = s; return d; },\n"
  "    // Type is one of:\n"
  "    // \"blur\"; \"change\"; \"click\"; \"dblclick\"; \"focus\"; \"input\";\n"
  "    // \"keydown\"; \"keypress\"; \"keyup\"; \"load\";\n"
  "    // \"mousedown\"; \"mousemove\"; \"mouseout\"; \"mouseover\"; \"mouseup\"; \"wheel\";\n"
  "    // \"select\"; \"selectstart\"; \"submit\"\n"
  "    // \\s, (\\->() | \\<event>->()) -> <domo>\n"
  "    on: (type, fn) => { e.addEventListener(type, fn, false); return d; },\n"
  "    // <domo> -> <domo>\n"
  "    remove: (o) => { e.removeChild(o.e); return d; },\n"
  "    // \\-> <domo>\n"
  "    removeAll: () => { e.innerHTML = \"\"; return d; },\n"
  "    // \\s, s-> <domo>\n"
  "    setStyle: (k, v) => { e.style.setProperty(k, v); return d; },\n"
  "    // \\s-> <domo>\n"
  "    style: (v) => { e.setAttribute(\"style\", v); return d; },\n"
  "    // \\s -> <domo>\n"
  "    text: (s) => { e.textContent = s; return d; },\n"
  "    // \\* -> <domo>\n"
  "    value: (v) => { e.value = v; return d; }\n"
  "  };\n"
  "  return d;\n"
  "}\n"
  "\n"
;}
