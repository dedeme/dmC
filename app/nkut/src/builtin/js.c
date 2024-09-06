// Copyright 06-Sep-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

char *js_bget (void) {return
  "// Copyright 28-Apr-2023 ºDeme\n"
  "// GNU General Public License - V3 <http://www.gnu.org/licenses/>\n"
  "\n"
  "import * as sys from './sys.js';\n"
  "\n"
  "// \\s -> *\n"
  "export function r (json) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  return JSON.parse(json);\n"
  "}\n"
  "\n"
  "// \\* -> s\n"
  "export function w (e) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  return JSON.stringify(e);\n"
  "}\n"
  "\n"
  "\n"
;}
