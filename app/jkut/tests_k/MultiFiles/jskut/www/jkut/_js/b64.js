// Copyright 17-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';
import * as str from './str.js';

// \s -> s
export function encode (s) {
  sys.$params(arguments.length, 1);
  return btoa(str.toUtf8(s));
}

// \<bytes> -> s
export function encodeBytes (Bs) {
  sys.$params(arguments.length, 1);
  const r = [];
  for (let i = 0; i < Bs.length; ++i) r.push(String.fromCharCode(Bs[i]));
  return btoa(r.join(""));
}

// \s -> s
export function decode (s) {
  sys.$params(arguments.length, 1);
  return str.fromUtf8(atob(s));
}

// \s -> <bytes>
export function decodeBytes (s) {
  sys.$params(arguments.length, 1);
  const tmp = atob(s);
  const r = [];
  r.length = tmp.length;
  for (let i = 0; i < r.length; ++i) r[i] = tmp.charCodeAt(i);
  return new Uint8Array(r);
}

