// Copyright 01-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';

// \n, n, n -> b
export function eq (n1, n2, gap) {
  sys.$params(arguments.length, 3);
  return n1 + gap >= n2 && n1 - gap <= n2;
}

// \s -> [n]|[]
export function fromEn (n) {
  sys.$params(arguments.length, 1);
  return fromStr(n.replace(",", ""));
}

// \s -> [n]|[]
export function fromHex (n) {
  sys.$params(arguments.length, 1);
  const n2 = n.toUpperCase();
  for (let i = 0; i < n2.length; +i++) {
    const ch = n2.charAt(i);
    if (!((ch >= "0" && ch <= "9") || (ch >= "A" && ch <= "F"))) return [];
  }
  return [parseInt(n2, 16)];
}

// \s -> [n]|[]
export function fromIso (n) {
  sys.$params(arguments.length, 1);
  return fromStr(n.replace(".", "").replace(",", "."));
}

// \s -> [n]|[]
export function fromStr (n) {
  sys.$params(arguments.length, 1);
  const v = parseFloat(n);
  if (isNaN(v) || v === Infinity || v === -Infinity) return [];
  return [v];
}

// \s -> b
export function isDigits (n) {
  sys.$params(arguments.length, 1);
  for (let i = 0; i < n.length; +i++) {
    const ch = n.charAt(i);
    if (!((ch >= "0" && ch <= "9"))) return false;
  }
  return true;
}

// \ -> n
export function rnd () {
  sys.$params(arguments.length, 0);
  return Math.random();
}

// \n -> n
export function rndi (top) {
  sys.$params(arguments.length, 1);
  return Math.trunc(Math.random() * Math.trunc(top));
}

// \n, n -> n
export function round (n, dec) {
  sys.$params(arguments.length, 2);
  function fpow (v) {
    switch (v) {
      case 2: return 100;
      case 0: return 1;
      case 1: return 10;
      case 3: return 1000;
      case 4: return 10000;
      case 5: return 100000;
      case 6: return 1000000;
      case 7: return 10000000;
      case 8: return 100000000;
      default: return 1000000000;
    }
  }
  if (dec < 0) dec = 0;
  else if (dec > 9) dec = 9;
  const mul = fpow(dec);
  if (n >= 0)
    return Math.round(n * mul + 0.000000001) / mul;
  return Math.round(n * mul - 0.000000001) / mul;
}

function to (n, dec, thsep, decsep) {
  const ps = toFix(n, dec).split(".");
  var ps0 = ps[0];
  var sign = "";
  if (ps0.charAt(0) == "-") {
    ps0 = ps0.substring(1);
    sign = "-";
  }
  const r0Length = ps0.length;
  const r0 = [...ps0].reverse();
  let r1 = [];
  let i = 0;
  while (true) {
    r1.push(r0[i]);
    ++i;
    if (i >= r0Length) break;
    if (i % 3 == 0) r1.push(thsep);
  }
  return sign + (ps.length == 1
    ? r1.reverse().join("")
    : r1.reverse().join("") + decsep + ps[1])
  ;
}

// \n, n -> s
export function toEn (n, dec) {
  sys.$params(arguments.length, 2);
  return to(n, dec, ",", ".");
}

// \n, n -> s
export function toFix (n, dec) {
  sys.$params(arguments.length, 2);
  function pad (s, n) {
    while (s.length < n) s += "0";
    return s;
  }

  if (n === -0) n = 0;
  const r1 = "" + round(n, dec);
  const ps = r1.split(".");
  if (ps.length == 1)
    return dec == 0 ? ps[0] : ps[0] + "." + pad("", dec);
  return ps[0] + "." + pad(ps[1].substring(0, dec), dec);
}

// \n -> s
export function toHex (n) {
  sys.$params(arguments.length, 1);
  return n.toString(16);
}

// \n, n -> s
export function toHexFix (n, dec) {
  sys.$params(arguments.length, 2);
  let r = n.toString(16);
  while (r.length < dec) r = "0" + r;
  return r;
}

// \n -> n
export function toInt (n) {
  sys.$params(arguments.length, 1);
  return Math.trunc(n);
}

// \n, n -> s
export function toIso(n, dec) {
  sys.$params(arguments.length, 2);
  return to(n, dec, ".", ",");
}

// \n -> s
export function toStr(n) {
  sys.$params(arguments.length, 1);
  if (n === -0) n = 0;
  return "" + n;
}


