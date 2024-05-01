// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';

// \s, s -> b
export function great (s1, s2) {
  sys.$params(arguments.length, 2);
  return s1.localeCompare(s2) > 0;
}

// \s, s -> b
export function ends (s, sub) {
  sys.$params(arguments.length, 2);
  return s.endsWith(sub);
}

// \s -> s
export function escape (s) {
  sys.$params(arguments.length, 1);
  const raw = /[\w*+\-./@]/;
  let r = [];
  let i = 0;
  while (i < s.length) {
    const ch = s.charAt(i++);
    if (raw.test(ch.charAt(0))) r.push(ch);
    else {
      const hex = ch.codePointAt(0).toString(16);
      switch (hex.length) {
        case 1:
        case 3:
          r.push('%0' + hex);
          break;
        default:
          r.push('%' + hex);
      }
    }
  }
  return r.join('');
}

// \s, a -> s
export function fmt (t, sb) {
  sys.$params(arguments.length, 2);
  if (t.length === 0) throw new Error("Template is empty");

  let ix = 0;
  let p = 0;
  let r = [];
  while (true) {
    const p2 = t.indexOf("%", p);
    if (p2 === -1) break;

    r.push(t.substring(p, p2));
    p = p2 + 1;

    if (p === t.length) throw new Error("Template ends at '%'");
    const ch = t.charAt(p);
    if (ch === "%") {
      r.push("%");
      ++p;
    } else if (ch === "v") {
      if (ix >= sb.length)
        throw new Error("Missing values replacing '" + t + "'");
      r.push(sys.toStr(sb[ix++]));
      ++p;
    } else {
      throw new Error("Character after '%' (" + ch + ") is not '%' nor 'v'");
    }
  }
  if (ix !== sb.length) throw new Error("Extra values replacing '" + t + "'");

  r.push(t.substring(p));
  return r.join("");
}

// \[n...] -> s
export function fromCodePoints (cps) {
  sys.$params(arguments.length, 1);
  return String.fromCodePoint(...cps);
}

// \s -> s
export function fromUtf8 (s) {
  sys.$params(arguments.length, 1);
  return decodeURIComponent(escape(s));
}

// \s, s -> n
export function index (s, sub) {
  sys.$params(arguments.length, 2);
  return s.indexOf(sub);
}

// \s, s, n -> n
export function indexFrom (s, sub, ix) {
  sys.$params(arguments.length, 3);
  return s.indexOf(sub, ix);
}

// \s, s -> n
export function lastIndex (s, sub) {
  sys.$params(arguments.length, 2);
  return s.lastIndexOf(sub);
}

// \s -> n
export function len (s) {
  sys.$params(arguments.length, 1);
  return s.length;
}

// \s, s -> b
export function less (s1, s2) {
  sys.$params(arguments.length, 2);
  return s1.localeCompare(s2) < 0;
}

// \s -> s
export function ltrim (s) {
  sys.$params(arguments.length, 1);
  return s.trimStart();
}

// \s, s, s -> s
export function replace (s, olds, news) {
  sys.$params(arguments.length, 3);
  return s.replaceAll(olds, news);
}

// \s -> s
export function rtrim (s) {
  sys.$params(arguments.length, 1);
  return s.trimEnd();
}

// \s, s -> [s...]
export function split (s, sep) {
  sys.$params(arguments.length, 2);
  return s.split(sep);
}

// \s, s -> [s...]
export function splitTrim (s, sep) {
  sys.$params(arguments.length, 2);
  return s.split(sep).map(e => e.trim());
}

// \s, s -> b
export function starts (s, sub) {
  sys.$params(arguments.length, 2);
  return s.startsWith(sub);
}

// \s -> [n...]
export function toCodePoints (s) {
  sys.$params(arguments.length, 1);
  const r = [];
  r.length = s.length;
  for (let i = 0; i < s.length; ++i) r[i] = s.codePointAt(i);
  return r;
}

// \s -> s
export function toLower(s) {
  sys.$params(arguments.length, 1);
  return s.toLowerCase();
}

// \s -> s
export function toUpper (s) {
  sys.$params(arguments.length, 1);
  return s.toUpperCase();
}

// \s -> s
export function toUtf8 (s) {
  sys.$params(arguments.length, 1);
  return unescape(encodeURIComponent(s));
}

// \s -> s
export function trim (s) {
  sys.$params(arguments.length, 1);
  return s.trim();
}

// \s -> s
export function unescape (s) {
  sys.$params(arguments.length, 1);
  return s.replace(
    /%([0-9A-Fa-f]{2})/g, (match, p1) => String.fromCharCode('0x' + p1)
  );
}


