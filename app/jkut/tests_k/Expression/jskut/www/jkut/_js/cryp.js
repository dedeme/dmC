// Copyright 17-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';
import * as b64 from './b64.js';

// \n -> s
export function genK (lg) {
  sys.$params(arguments.length, 1);
  const bs = new Uint8Array(lg);
  for (let i = 0; i < lg; ++i) bs[i] = Math.floor(Math.random() * 256);
  return b64.encodeBytes(bs).substring(0, lg);
}

// \s, n -> s
export function key (key, lg) {
  sys.$params(arguments.length, 2);
    const k = b64.decodeBytes(b64.encode(
      key + "codified in irreversibleDeme is good, very good!\n\r8@@"
    ));
    const lenk = k.length;
    let sum = 0;
    for (let i = 0; i < lenk; ++i) sum += k[i];

    const lg2 = lg + lenk;
    const r = new Uint8Array(lg2);
    const r1 = new Uint8Array(lg2);
    const r2 = new Uint8Array(lg2);
    let ik = 0;
    for (let i = 0; i < lg2; ++i) {
      const v1 = k[ik];
      const v2 = v1 + k[v1 % lenk];
      const v3 = v2 + k[v2 % lenk];
      const v4 = v3 + k[v3 % lenk];
      sum = sum + i + v4;
      r1[i] = sum;
      r2[i] = sum;
      ++ik;
      if (ik == lenk) ik = 0;
    }

    for (let i = 0; i < lg2; ++i) {
      const v1 = r2[i];
      const v2 = v1 + r2[v1 % lg2];
      const v3 = v2 + r2[v2 % lg2];
      const v4 = v3 + r2[v3 % lg2];
      sum = sum + v4;
      r2[i] = sum;
      r[i] = sum + r1[i];
    }

    return b64.encodeBytes(r).substring(0, lg);
  }

// \s, s -> s
export function encode (k, msg) {
  sys.$params(arguments.length, 2);
  const m = b64.encode(msg);
  const lg = m.length;
  const k2 = key(k, lg);
  const r = new Uint8Array(lg);
  for (let i = 0; i < lg; ++i) r[i] = m.charCodeAt(i) + k2.charCodeAt(i);

  return b64.encodeBytes(r);
}

// \s, s -> s
export function decode (k, b64Tx) {
  sys.$params(arguments.length, 2);
  const bs = b64.decodeBytes(b64Tx);
  const lg = bs.length;
  const k2 = key(k, lg);
  const r = [];
  for (let i = 0; i < lg; ++i)
    r.push(String.fromCharCode(bs[i] - k2.charCodeAt(i)));

  return b64.decode(r.join(""));
}

