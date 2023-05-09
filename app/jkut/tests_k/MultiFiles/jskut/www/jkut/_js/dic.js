// Copyright 01-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';
import * as arr from './arr.js';
import * as iter from './iter.js';

// \d -> d
export function copy (d) {
  sys.$params(arguments.length, 1);
  const r = {};
  for (const k in d) r[k] = d[k];
  return r;
}

// \[[s,*]...] -> d
export function fromArr (a) {
  sys.$params(arguments.length, 1);
  const r = {};
  for (const kv of a) r[kv[0]] = kv[1];
  return r;
}

// \<iter>[[s,*]...] -> d
export function fromIter (it) {
  sys.$params(arguments.length, 1);
  const r = {};
  while (iter.hasNext(it)) {
    const kv = iter.next(it);
    r[kv[0]] = kv[1];
  }
  return r;
}

// \d, s -> []|[*]
export function get (d, k) {
  sys.$params(arguments.length, 2);
  return sys.$null(d[k]);
}

// \d, s -> b
export function hasKey (d, k) {
  sys.$params(arguments.length, 2);
  return d.propertyIsEnumerable(k);
}

// \d -> [s...]
export function keys (d) {
  sys.$params(arguments.length, 1);
  return Object.keys(d);
}

// \d, s, * -> ()
export function put (d, k, v) {
  sys.$params(arguments.length, 3);
  return d[k] = v;
}

// \d, s -> ()
export function remove (d, k) {
  sys.$params(arguments.length, 2);
  delete d[k];
}

// \d -> n
export function size (d) {
  sys.$params(arguments.length, 1);
  return Object.keys(d).length;
}

// \d -> [[s,*]...]
export function toArr (d) {
  sys.$params(arguments.length, 1);
  const r = [];
  const keys = Object.keys(d);
  r.length = keys.length;
  for (let i = 0; i < keys.length; ++i) {
    const k = keys[i];
    r[i] = [k, d[k]];
   }
  return r;
}

// \d -> <iter>[[s,*]...]
export function toIter (d) {
  sys.$params(arguments.length, 1);
  const keys = Object.keys(d);
  let ix = 0;
  return iter.mk(
    () => ix < keys.length,
    () => {
        const k = keys[ix++];
        return [k, d[k]];
      }
  );
}

// \d -> a
export function values (d) {
  sys.$params(arguments.length, 1);
  const r = [];
  const keys = Object.keys(d);
  r.length = keys.length;
  for (let i = 0; i < keys.length; ++i) r[i] = d[keys[i]];
  return r;
}


