// Copyright 04-Sep-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

char *path_bget (void) {return
  "// Copyright 29-Jun-2024 ºDeme\n"
  "// GNU General Public License - V3 <http://www.gnu.org/licenses/>\n"
  "\n"
  "import * as sys from './sys.js';\n"
  "import {realpathSync} from \"fs\";\n"
  "\n"
  "function normalize(p) {\n"
  "  let r = p.trim();\n"
  "  while (r.endsWith(\"/\")) r = r.substring(0, r.lenght - 1);\n"
  "  return r;\n"
  "}\n"
  "\n"
  "// \\s -> s\n"
  "export function base (p) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  const r = normalize(p);\n"
  "  const ix = r.lastIndexOf('/');\n"
  "  if (ix !== -1) return r.substring(ix + 1);\n"
  "  return r;\n"
  "}\n"
  "\n"
  "// \\s -> s\n"
  "export function canonical (p) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  return realpathSync(p);\n"
  "}\n"
  "\n"
  "// \\[s.] -> s\n"
  "export function cat (a) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  if (a.length === 0) a.push(\"./\");\n"
  "  else if (a[0] === \"\") a[0] = \".\";\n"
  "  return clean(a.join(\"/\"));\n"
  "}\n"
  "\n"
  "// \\s -> s\n"
  "export function clean (p) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  let s = p.trim();\n"
  "  if (s === \"\") return \"\";\n"
  "  let bf = [];\n"
  "  let isSlash = false;\n"
  "  for (let i = 0; i < s.length; ++i) {\n"
  "    const ch = s.charAt(i);\n"
  "    if (ch === \"/\") {\n"
  "      if (isSlash) continue;\n"
  "      isSlash = true;\n"
  "      bf.push(ch);\n"
  "      continue;\n"
  "    }\n"
  "    isSlash = false;\n"
  "    bf.push(ch);\n"
  "  }\n"
  "  s = bf.join(\"\");\n"
  "  if (s.length === 1) return s;\n"
  "  if (s.endsWith(\"/\")) s = s.substring(0, s.length - 1);\n"
  "\n"
  "  bf = [];\n"
  "  const parts = s.split(\"/\");\n"
  "  for (let i = 0; i < parts.length; ++i) {\n"
  "    const part = parts[i];\n"
  "    if (part === \".\") continue;\n"
  "    if (part === \"..\") {\n"
  "      if (bf.length > 0) bf.pop();\n"
  "      else throw new Error(\"Bad path for cleaning '\" + p + \"'\");\n"
  "      continue;\n"
  "    }\n"
  "    bf.push(part);\n"
  "  }\n"
  "  s = bf.join(\"/\");\n"
  "  if (s === \"\" && p[0] === \"/\")\n"
  "    throw new Error(\"Bad path for cleaning '\" + p + \"'\");\n"
  "\n"
  "  return s;\n"
  "}\n"
  "\n"
  "// \\s -> s\n"
  "export function extension (p) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  const r = base(p);\n"
  "  const ix = r.lastIndexOf(\".\");\n"
  "  if (ix === -1) return \"\";\n"
  "  return r.substring(ix);\n"
  "}\n"
  "\n"
  "// \\s -> s\n"
  "export function parent (p) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  const r = normalize(p);\n"
  "  if (r === \"\") throw new Error(\"'\" + p + \"' has not parent directory\");\n"
  "  const ix = r.lastIndexOf(\"/\");\n"
  "  if (ix === -1) return \"\";\n"
  "  if (ix === 0) return \"/\";\n"
  "  return r.substring(0, ix);\n"
  "}\n"
  "\n"
  "\n"
;}