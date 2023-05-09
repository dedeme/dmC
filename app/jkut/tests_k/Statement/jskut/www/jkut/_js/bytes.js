// Copyright 17-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Uint8Array

import * as sys from './sys.js';
import * as str from './str.js';

// \<bytes>, <bytes> -> <bytes>
export function add (bs1, bs2) {
  sys.$params(arguments.length, 2);
  const bs = new Uint8Array(bs1.length + bs2.length);
  bs.set(bs1, 0);
  bs.set(bs2, bs1.length);
  return bs;
}

// \a, n -> a
export function drop (bs, n) {
  sys.$params(arguments.length, 2);
  if (n < 0) n = 0;
  return bs.slice(n);
}

// \[n...] -> <bytes>
export function fromArr (a) {
  sys.$params(arguments.length, 1);
  return new Uint8Array(a);
}

// \s -> <bytes>
export function fromStr (s) {
  sys.$params(arguments.length, 1);
  const r = [];
  for (const ch of str.toUtf8(s)) r.push(ch.codePointAt(0));
  return new Uint8Array(r);
}

// \n -> <bytes>
export function mk (n) {
  sys.$params(arguments.length, 1);
  if (n < 0) n = 0;
  return new Uint8Array(n)
}

// \<bytes> -> n
export function size (bs) {
  sys.$params(arguments.length, 1);
  return bs.length;
}

// \a, n -> a
export function take (bs, n) {
  sys.$params(arguments.length, 2);
  if (n < 0) n = 0;
  return bs.slice(0, n);
}

// \<bytes> -> s
export function toArr (bs) {
  sys.$params(arguments.length, 1);
  const r = [];
  r.length = bs.length;
  for (let i = 0; i < bs.length; ++i) r[i] = bs[i];
  return r;
}

// \<bytes> -> s
export function toStr (bs) {
  sys.$params(arguments.length, 1);
  const r = [];
  bs.forEach(e => r.push(String.fromCodePoint(e)));
  return str.fromUtf8(r.join(''));
}

