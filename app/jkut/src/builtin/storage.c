// Copyright 30-Jul-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

char *storage_bget (void) {return
  "// Copyright 05-May-2023 ºDeme\n"
  "// GNU General Public License - V3 <http://www.gnu.org/licenses/>\n"
  "\n"
  "import * as sys from './sys.js';\n"
  "import * as iter from './iter.js';\n"
  "\n"
  "// \\s -> ()\n"
  "export function del (s) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  localStorage.removeItem(s);\n"
  "}\n"
  "\n"
  "// \\s -> ()\n"
  "export function get (s) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  const r = localStorage.getItem(s);\n"
  "  return r == null ? [] : [r];\n"
  "}\n"
  "\n"
  "// \\-> <iter>[s...]\n"
  "export function keys () {\n"
  "  sys.$params(arguments.length, 0);\n"
  "  let i = 0;\n"
  "  return iter.mk(\n"
  "    () => i < localStorage.length,\n"
  "    () => localStorage.key(i++)\n"
  "  );\n"
  "}\n"
  "\n"
  "// \\s, s -> ()\n"
  "export function put (key, value) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  localStorage.setItem(key, value);\n"
  "}\n"
  "\n"
  "\n"
;}