// Copyright 15-Feb-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

char *iter_bget (void) {return
  "// Copyright 16-Apr-2023 ºDeme\n"
  "// GNU General Public License - V3 <http://www.gnu.org/licenses/>\n"
  "\n"
  "import * as sys from './sys.js';\n"
  "import * as arr from './arr.js';\n"
  "\n"
  "export function $range (begin, end) {\n"
  "  let ix = begin;\n"
  "  return mk (\n"
  "    () => { return ix < end ? true : false; },\n"
  "    () => { return ix++; }\n"
  "  );\n"
  "}\n"
  "\n"
  "export function $range_step (begin, end, step) {\n"
  "  let ix = begin;\n"
  "  if (step > 0) {\n"
  "    return mk (\n"
  "      () => { return ix <= end ? true : false; },\n"
  "      () => {\n"
  "        const r = ix;\n"
  "        ix += step;\n"
  "        return r;\n"
  "      }\n"
  "    );\n"
  "  } else {\n"
  "    return mk (\n"
  "      () => { return ix >= end ? true : false; },\n"
  "      () => {\n"
  "        const r = ix;\n"
  "        ix += step;\n"
  "        return r;\n"
  "      }\n"
  "    );\n"
  "  }\n"
  "}\n"
  "\n"
  "// Built-in --------------------\n"
  "\n"
  "// \\<iter>, (\\*->b) -> b\n"
  "export function all (it, fn) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  sys.$fparams(fn, 1);\n"
  "  while (hasNext(it)) if (!fn(next(it))) return false;\n"
  "  return true;\n"
  "}\n"
  "\n"
  "// \\<iter>, (\\*->b) -> b\n"
  "export function any (it, fn) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  sys.$fparams(fn, 1);\n"
  "  while (hasNext(it)) if (fn(next(it))) return true;\n"
  "  return false;\n"
  "}\n"
  "\n"
  "// \\<iter>, <iter> -> <iter>\n"
  "export function cat (it1, it2) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  return mk(\n"
  "    () => hasNext(it1) || hasNext(it2),\n"
  "    () => hasNext(it1) ? next(it1) : next(it2)\n"
  "  );\n"
  "}\n"
  "\n"
  "// \\<iter> -> n\n"
  "export function count (it) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  let n = 0;\n"
  "  while (hasNext(it)) {\n"
  "    ++n\n"
  "    next(it);\n"
  "  }\n"
  "  return n;\n"
  "}\n"
  "\n"
  "// \\<iter>, n -> <iter>\n"
  "export function drop (it, n) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  if (n < 0) n = 0;\n"
  "  let c = 0;\n"
  "  while (c < n && hasNext(it)) {\n"
  "    ++c\n"
  "    next(it);\n"
  "  }\n"
  "  return it;\n"
  "}\n"
  "\n"
  "// \\<iter>, (\\*->b) -> <iter>\n"
  "export function dropWhile (it, fn) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  sys.$fparams(fn, 1);\n"
  "  let it0 = empty();\n"
  "  while (hasNext(it)) {\n"
  "    const e = next(it);\n"
  "    if (!fn(e)) {\n"
  "      it0 = unary(e);\n"
  "      break;\n"
  "    }\n"
  "  }\n"
  "  return cat(it0, it);\n"
  "}\n"
  "\n"
  "// \\<iter>, (\\*->()) -> ()\n"
  "export function each (it, fn) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  sys.$fparams(fn, 1);\n"
  "  while (hasNext(it)) fn(next(it));\n"
  "}\n"
  "\n"
  "// \\<iter>, (\\*,n->()) -> ()\n"
  "export function eachIx (it, fn) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  sys.$fparams(fn, 2);\n"
  "  let ix = 0;\n"
  "  while (hasNext(it)) fn(next(it), ix++);\n"
  "}\n"
  "\n"
  "// \\<iter>, (async \\n,*->()), (\\->()) -> ()\n"
  "export function eachSync (it, f1, f2) {\n"
  "  sys.$params(arguments.length, 3);\n"
  "  sys.$fparams(f1, 2);\n"
  "  sys.$fparams(f2, 0);\n"
  "  let ix = 0;\n"
  "  async function fn () {\n"
  "    if (hasNext(it)) {\n"
  "      await f1(ix++, next(it));\n"
  "      fn();\n"
  "    } else {\n"
  "      f2();\n"
  "    }\n"
  "  }\n"
  "  fn();\n"
  "}\n"
  "\n"
  "// \\-> <iter>\n"
  "export function empty (it, fn) {\n"
  "  sys.$params(arguments.length, 0);\n"
  "  return mk(\n"
  "    () => false,\n"
  "    () => null\n"
  "  );\n"
  "}\n"
  "\n"
  "// \\<iter>, (\\*->b) -> <iter>\n"
  "export function filter (it, fn) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  sys.$fparams(fn, 1);\n"
  "  let has = false;\n"
  "  let e = null;\n"
  "  while (hasNext(it)) {\n"
  "    e = next(it);\n"
  "    if (fn(e)) {\n"
  "      has = true;\n"
  "      break\n"
  "    }\n"
  "  }\n"
  "\n"
  "  return mk(\n"
  "    () => has,\n"
  "    () => {\n"
  "      const r = e;\n"
  "      has = false;\n"
  "      while (hasNext(it)) {\n"
  "        e = next(it);\n"
  "        if (fn(e)) {\n"
  "          has = true;\n"
  "          break\n"
  "        }\n"
  "      }\n"
  "      return r;\n"
  "    }\n"
  "  );\n"
  "}\n"
  "\n"
  "// \\<iter>, (\\*->b) -> ([] | [*])\n"
  "export function find (it, fn) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  sys.$fparams(fn, 1);\n"
  "  while (hasNext(it)) {\n"
  "    const e = next(it);\n"
  "    if (fn(e)) return [e];\n"
  "  }\n"
  "  return [];\n"
  "}\n"
  "\n"
  "// \\<iter>, (\\*->b) -> n\n"
  "export function index (it, fn) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  sys.$fparams(fn, 1);\n"
  "  let ix = 0;\n"
  "  while (hasNext(it)) {\n"
  "    if (fn(next(it))) return ix;\n"
  "    ++ix;\n"
  "  }\n"
  "  return -1;\n"
  "}\n"
  "\n"
  "// \\<iter>, (\\*->*) -> <iter>\n"
  "export function map (it, fn) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  sys.$fparams(fn, 1);\n"
  "  return mk(\n"
  "    () => hasNext(it),\n"
  "    () => fn(next(it))\n"
  "  );\n"
  "}\n"
  "\n"
  "// \\<iter> -> b\n"
  "export function hasNext (it) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  return it[0]();\n"
  "}\n"
  "\n"
  "// \\(()->b), (()->*) -> <iter>\n"
  "export function mk (hasNext, next) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  const r = [hasNext, next];\n"
  "  r[Symbol.iterator] = function* () {\n"
  "    while (hasNext()) yield next();\n"
  "    return 0;\n"
  "  }\n"
  "  return r;\n"
  "}\n"
  "\n"
  "// \\<iter> -> *\n"
  "export function next (it) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  return it[1]();\n"
  "}\n"
  "\n"
  "// \\<iter>, *, (\\*,*->*) -> *\n"
  "export function reduce (it, seed, fn) {\n"
  "  sys.$params(arguments.length, 3);\n"
  "  while (hasNext(it)) seed = fn(seed, next(it));\n"
  "  return seed;\n"
  "}\n"
  "\n"
  "// \\<iter>, n -> <iter>\n"
  "export function take (it, n) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  if (n < 0) n = 0;\n"
  "  let c = 0;\n"
  "  return mk(\n"
  "    () => c < n && hasNext(it),\n"
  "    () => {\n"
  "        ++c;\n"
  "        return next(it);\n"
  "      }\n"
  "  );\n"
  "}\n"
  "\n"
  "// \\<iter>, (\\*->b) -> <iter>\n"
  "export function takeWhile (it, fn) {\n"
  "  sys.$params(arguments.length, 2);\n"
  "  sys.$fparams(fn, 1);\n"
  "  let has = false;\n"
  "  let e = null;\n"
  "  if (hasNext(it)) {\n"
  "    e = next(it);\n"
  "    if (fn(e)) has = true;\n"
  "  }\n"
  "  return mk(\n"
  "    () => has,\n"
  "    () => {\n"
  "        const r = e;\n"
  "        has = false;\n"
  "        if (hasNext(it)) {\n"
  "          e = next(it);\n"
  "          if (fn(e)) has = true;\n"
  "        }\n"
  "        return r;\n"
  "      }\n"
  "  );\n"
  "}\n"
  "\n"
  "// \\<iter> -> s\n"
  "export function toStr (it) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  return \"<iter>\" + sys.toStr(arr.fromIter(it));\n"
  "}\n"
  "\n"
  "// \\* -> <iter>\n"
  "export function unary (e) {\n"
  "  sys.$params(arguments.length, 1);\n"
  "  let has = true;\n"
  "  return mk (\n"
  "    () => has,\n"
  "    () => {\n"
  "      has = false;\n"
  "      return e;\n"
  "    }\n"
  "  );\n"
  "}\n"
  "\n"
  "\n"
  "\n"
  "\n"
;}
