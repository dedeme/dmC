// Copyright 28-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';
import * as iter from './iter.js';

// \a, (\*->b) -> b
export function all (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  return a.every(e => fn(e));
}

// \a, (\*->b) -> b
export function any (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  return a.some(e => fn(e));
}

// \a, a -> ()
export function cat (a1, a2) {
  sys.$params(arguments.length, 2);
  a1.push(...a2);
}

// \a -> ()
export function clear (a) {
  sys.$params(arguments.length, 1);
  a.length = 0;
}

// \[a.] -> a
export function concat (a) {
  sys.$params(arguments.length, 1);
  let r = [];
  for (const e of a) r = r.concat(e);
  return r;
}

// \a -> a
export function copy (a) {
  sys.$params(arguments.length, 1);
  return a.slice();
}

// \a, n -> a
export function drop (a, n) {
  sys.$params(arguments.length, 2);
  if (n < 0) n = 0;
  return a.slice(n);
}

// \a, (\*->b) -> a
export function dropWhile (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  const n = a.findIndex((e) => !fn(e));
  return n == -1 ? [] : a.slice(n);
}

// \a, (\*,*->b) -> [a, a]
export function duplicates (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 2);
  const rs = [];
  const dup = [];
  for (const e1 of a)
    if (rs.some(e2 => fn(e1, e2))) {
      if (!dup.some(e3 => fn(e1, e3))) dup.push(e1);
    } else {
      rs.push(e1);
    }
  return [rs, dup];
}

// \a, (\*->()) -> ()
export function each (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  a.forEach(e => fn(e));
}

// \a, (\*,n->()) -> ()
export function eachIx (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 2);
  a.forEach((e, i) => fn(e, i));
}

// \a, (async \n,*->()), (\->()) -> ()
export function eachSync (a, f1, f2) {
  sys.$params(arguments.length, 3);
  sys.$fparams(f1, 2);
  sys.$fparams(f2, 0);
  let ix = 0;
  let len = a.length;
  async function fn () {
    if (ix < len) {
      await f1(ix, a[ix]);
      ++ix;
      fn();
    } else {
      f2();
    }
  }
  fn();
}

// \a, (\*->()) -> ()
export function filter (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  return a.filter(e => fn(e));
}

// \a, (\*->()) -> ()
export function filterIn (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  let ni = 0;
  for (let i = 0; i < a.length; ++i) {
    if (fn(a[i])) {
      a[ni] = a[i];
      ++ni;
    }
  }
  a.length = ni;
}

// \a, (\*->()) -> ([*] | [])
export function find (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  return sys.$null(a.find(e => fn(e)));
}

// \<iter> -> a
export function fromIter (it) {
  sys.$params(arguments.length, 1);
  const r = [];
  while (iter.hasNext(it)) r.push(iter.next(it));
  return r;
}

// \a, (\*->b) -> n
export function index (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  return a.findIndex(e => fn(e));
}

// \a, n, * -> ()
export function insert (a, ix, e) {
  sys.$params(arguments.length, 3);
  a.splice(ix, 0, e);
}

// \a, n, * -> ()
export function insertArr (a, ix, a2) {
  sys.$params(arguments.length, 3);
  a.splice(ix, 0, ...a2);
}

// \[s...], s -> s
export function join (a, sp) {
  sys.$params(arguments.length, 2);
  return a.join(sp);
}

// \a, (\*->*) -> a
export function map (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  const r = [];
  r.length = a.length;
  for (let i = 0; i < a.length; ++i) r[i] = fn(a[i]);
  return r;
}

// \n, * -> a
export function mk (n, e) {
  sys.$params(arguments.length, 2);
  if (n < 0) n == 0;
  const r = [];
  r.length = n;
  if (typeof(e) === "object") {
    if (Array.isArray(e))
      for (let i = 0; i < n; ++i) r[i] = e.slice();
    else
      throw new Error(
        '\nExpected: boolean, number, string or Array.\n   Found: ' + o
      );
  } else {
    for (let i = 0; i < n; ++i) r[i] = e;
  }
  return r;
}

// \a -> *
export function peek (a) {
  sys.$params(arguments.length, 1);
  if (a.length == 0) throw new Error("Array is empty");
  return a[a.length - 1];
}

// \a -> *
export function pop (a) {
  sys.$params(arguments.length, 1);
  if (a.length == 0) throw new Error("Array is empty");
  return a.pop();
}

// \a, * -> ()
export function push (a, e) {
  sys.$params(arguments.length, 2);
  a.push(e);
}

// \a, *, (\*,*->*) -> *
export function reduce (a, seed, fn) {
  sys.$params(arguments.length, 3);
  sys.$fparams(fn, 2);
  return a.reduce((r, e, v1, v2) => fn(r,e), seed);
}

// \a, n -> *
export function remove (a, ix) {
  sys.$params(arguments.length, 2);
  const r = a[ix];
  if (r !== undefined) a.splice(ix, 1);
  return r;
}

// \a, n -> *
export function removeRange (a, begin, end) {
  sys.$params(arguments.length, 2);
  if (begin > end) return null;
  if (begin < 0) return null;
  if (end > a.length) return null;
  const r = a.slice(begin, end);
  a.splice(begin, end - begin);
  return r;
}

// \a -> a
export function reverse (a) {
  sys.$params(arguments.length, 1);
  return a.slice().reverse();
}

// \a -> ()
export function reverseIn (a) {
  sys.$params(arguments.length, 1);
  a.reverse();
}

// \a -> *
export function shift (a) {
  sys.$params(arguments.length, 1);
  if (a.length == 0) throw new Error("Array is empty");
  return a.shift();
}

// \a -> ()
export function shuffle (a) {
  sys.$params(arguments.length, 1);
  for (let i = a.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [a[i], a[j]] = [a[j], a[i]];
  }
}

// \a -> n
export function size (a) {
  sys.$params(arguments.length, 1);
  return a.length;
}

// (\a, \*,*->b) -> ()
export function sort (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 2);
  a.sort((a, b) => fn(a, b) ? -1 : 1);
}

// \a, n -> a
export function take (a, n) {
  sys.$params(arguments.length, 2);
  if (n < 0) n = 0;
  return a.slice(0, n);
}

// \a, (\*->b) -> a
export function takeWhile (a, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  const n = a.findIndex((e) => !fn(e));
  return n == -1 ? a.slice() : a.slice(0, n);
}

// \a -> <iter>
export function toIter (a) {
  sys.$params(arguments.length, 1);
  let ix = 0;
  return iter.mk(
    () => ix < a.length,
    () => a[ix++]
  );
}

// \a, * -> ()
export function unshift (a, e) {
  sys.$params(arguments.length, 2);
  a.unshift(e);
}

/// \[[*.].] -> [[*.].]
export function zip (a) {
  sys.$params(arguments.length, 1);
  if (a.length < 2) return a;
  let nels = a[0].length;
  for (let i = 1; i < a.length; ++i)
    if (a[i].length < nels) nels = a[i].length;
  const r = [];
  for (let i = 0; i < nels; ++i) {
    const row = [];
    for (let j = 0; j < a.length; ++j) row.push(a[j][i]);
    r.push(row);
  }
  return r;
}

