// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';
import * as arr from './arr.js';

export function $range (begin, end) {
  let ix = begin;
  return mk (
    () => { return ix < end ? true : false; },
    () => { return ix++; }
  );
}

export function $range_step (begin, end, step) {
  let ix = begin;
  if (step > 0) {
    return mk (
      () => { return ix <= end ? true : false; },
      () => {
        const r = ix;
        ix += step;
        return r;
      }
    );
  } else {
    return mk (
      () => { return ix >= end ? true : false; },
      () => {
        const r = ix;
        ix += step;
        return r;
      }
    );
  }
}

// Built-in --------------------

// \<iter>, (\*->b) -> b
export function all (it, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  while (hasNext(it)) if (!fn(next(it))) return false;
  return true;
}

// \<iter>, (\*->b) -> b
export function any (it, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  while (hasNext(it)) if (fn(next(it))) return true;
  return false;
}

// \<iter>, <iter> -> <iter>
export function cat (it1, it2) {
  sys.$params(arguments.length, 2);
  return mk(
    () => hasNext(it1) || hasNext(it2),
    () => hasNext(it1) ? next(it1) : next(it2)
  );
}

// \<iter> -> n
export function count (it) {
  sys.$params(arguments.length, 1);
  let n = 0;
  while (hasNext(it)) {
    ++n
    next(it);
  }
  return n;
}

// \<iter>, n -> <iter>
export function drop (it, n) {
  sys.$params(arguments.length, 2);
  if (n < 0) n = 0;
  let c = 0;
  while (c < n && hasNext(it)) {
    ++c
    next(it);
  }
  return it;
}

// \<iter>, (\*->b) -> <iter>
export function dropWhile (it, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  let it0 = empty();
  while (hasNext(it)) {
    const e = next(it);
    if (!fn(e)) {
      it0 = unary(e);
      break;
    }
  }
  return cat(it0, it);
}

// \<iter>, (\*->()) -> ()
export function each (it, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  while (hasNext(it)) fn(next(it));
}

// \<iter>, (\*,n->()) -> ()
export function eachIx (it, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 2);
  let ix = 0;
  while (hasNext(it)) fn(next(it), ix++);
}

// \-> <iter>
export function empty (it, fn) {
  sys.$params(arguments.length, 0);
  return mk(
    () => false,
    () => null
  );
}

// \<iter>, (\*->b) -> <iter>
export function filter (it, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  let has = false;
  let e = null;
  while (hasNext(it)) {
    e = next(it);
    if (fn(e)) {
      has = true;
      break
    }
  }

  return mk(
    () => has,
    () => {
      const r = e;
      has = false;
      while (hasNext(it)) {
        e = next(it);
        if (fn(e)) {
          has = true;
          break
        }
      }
      return r;
    }
  );
}

// \<iter>, (\*->b) -> ([] | [*])
export function find (it, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  while (hasNext(it)) {
    const e = next(it);
    if (fn(e)) return [e];
  }
  return [];
}

// \<iter>, (\*->b) -> n
export function index (it, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  let ix = 0;
  while (hasNext(it)) {
    if (fn(next(it))) return ix;
    ++ix;
  }
  return -1;
}

// \<iter>, (\*->*) -> <iter>
export function map (it, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  return mk(
    () => hasNext(it),
    () => fn(next(it))
  );
}

// \<iter> -> b
export function hasNext (it) {
  sys.$params(arguments.length, 1);
  return it[0]();
}

// \(()->b), (()->*) -> <iter>
export function mk (hasNext, next) {
  sys.$params(arguments.length, 2);
  const r = [hasNext, next];
  r[Symbol.iterator] = function* () {
    while (hasNext()) yield next();
    return 0;
  }
  return r;
}

// \<iter> -> *
export function next (it) {
  sys.$params(arguments.length, 1);
  return it[1]();
}

// \<iter>, *, (\*,*->*) -> *
export function reduce (it, seed, fn) {
  sys.$params(arguments.length, 3);
  while (hasNext(it)) seed = fn(seed, next(it));
  return seed;
}

// \<iter>, n -> <iter>
export function take (it, n) {
  sys.$params(arguments.length, 2);
  if (n < 0) n = 0;
  let c = 0;
  return mk(
    () => c < n && hasNext(it),
    () => {
        ++c;
        return next(it);
      }
  );
}

// \<iter>, (\*->b) -> <iter>
export function takeWhile (it, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);
  let has = false;
  let e = null;
  if (hasNext(it)) {
    e = next(it);
    if (fn(e)) has = true;
  }
  return mk(
    () => has,
    () => {
        const r = e;
        has = false;
        if (hasNext(it)) {
          e = next(it);
          if (fn(e)) has = true;
        }
        return r;
      }
  );
}

// \<iter> -> s
export function toStr (it) {
  sys.$params(arguments.length, 1);
  return "<iter>" + sys.toStr(arr.fromIter(it));
}

// \* -> <iter>
export function unary (e) {
  sys.$params(arguments.length, 1);
  let has = true;
  return mk (
    () => has,
    () => {
      has = false;
      return e;
    }
  );
}




