// Copyright 02-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';
import * as math from './math.js';

// \n, n -> n
export function addDays (t, n) {
  sys.$params(arguments.length, 2);
  const dt = new Date(t);
  dt.setDate(dt.getDate() + n)
  return dt.getTime();
}

// \n -> n
export function day (t) {
  sys.$params(arguments.length, 1);
  return new Date(t).getDate();
}

// \n, n -> n
export function dfDays (t1, t2) {
  sys.$params(arguments.length, 2);
  const df = Math.trunc((t1 / 86400000) - (t2 / 86400000));
  const t1b = addDays(t2, df);
  return day(t1b) != day(t1)
    ? t1b > t1 ? df + 1 : df - 1
    : df
  ;
}

// \n, n -> b
export function eqDay (t1, t2) {
  sys.$params(arguments.length, 2);
  return dfDays(t1, t2) === 0;
}

// \s, n -> s
export function fmt (template, t) {
  sys.$params(arguments.length, 2);

  function f2 (n) {
    const s = "" + n;
    return s.length == 2 ? s : "0" + s;
  }

  if (template.length === 0) return "";

  let dt = new Date(t);
  let p = 0;
  let r = [];
  while (true) {
    const p2 = template.indexOf("%", p);
    if (p2 === -1) break;

    r.push(template.substring(p, p2));
    p = p2 + 1;

    if (p === t.length) throw new Error("Template ends at '%'");
    switch (template.charAt(p)) {
      case "%":
        r.push("%");
        break;
      case "d":
        r.push(dt.getDate());
        break;
      case "D":
        r.push(f2(dt.getDate()));
        break;
      case "m":
        r.push(dt.getMonth() + 1);
        break;
      case "M":
        r.push(f2(dt.getMonth() + 1));
        break;
      case "y":
        r.push(dt.toISOString().substring(2, 4));
        break;
      case "Y":
        r.push(dt.toISOString().substring(0, 4));
        break;
      case "t":
        r.push(
          f2(dt.getHours()) + ":" +
          f2(dt.getMinutes()) + ":" +
          f2(dt.getSeconds())
        );
        break;
      case "T":
        r.push(
          f2(dt.getHours()) + ":" +
          f2(dt.getMinutes()) + ":" +
          f2(dt.getSeconds()) + "." +
          dt.getMilliseconds()
        );
        break;
      default:
        throw new Error("Character after '%' (" + ch + ") is not valid");
    }
    ++p;
  }

  r.push(template.substring(p));
  return r.join("");
}

// \n, s -> n
export function fromClock (t1, s) {
  sys.$params(arguments.length, 2);
  const r = fromClockOp(t1, s);
  if (r.length === 0) throw new Error("Bad template '" + s + "'");
  return r[0];
}

// \n, s -> [n] | []
export function fromClockOp (t1, s) {
  sys.$params(arguments.length, 2);
  if (s.charAt(2) !== ":" || s.charAt(5) !== ":") return [];
  s = s.substring(0, 2) + s.substring(3,5) + s.substring(6);
  if (!math.isDigits(s) || s.length != 6) return [];
  const h = s.substring(0, 2)|0;
  const m = s.substring(2, 4)|0;
  const sec = s.substring(4)|0;
  if (h > 23 || m > 59 || sec > 59) return [];
  const dt = new Date(t1);
  dt.setHours(h);
  dt.setMinutes(m);
  dt.setSeconds(sec);
  dt.setMilliseconds(0);
  return [dt.getTime()];
}

// \s, s -> n
export function fromEn (s, sep) {
  sys.$params(arguments.length, 2);
  const r = fromEnOp(s, sep);
  if (r.length === 0) throw new Error("Bad template '" + s + "'");
  return r[0];
}

// \s, s -> [n] | []
export function fromEnOp (s, sep) {
  sys.$params(arguments.length, 2);
  if (s.charAt(2) !== sep.charAt(0) || s.charAt(5) !== sep.charAt(0)) return [];
  s = s.substring(6) + s.substring(0, 2) + s.substring(3,5);
  return fromStrOp(s);
}

// \s, s -> n
export function fromIso (s, sep) {
  sys.$params(arguments.length, 2);
  const r = fromIsoOp(s, sep);
  if (r.length === 0) throw new Error("Bad template '" + s + "'");
  return r[0];
}

// \s, s -> [n] | []
export function fromIsoOp (s, sep) {
  sys.$params(arguments.length, 2);
  if (s.charAt(2) !== sep.charAt(0) || s.charAt(5) !== sep.charAt(0)) return [];
  s = s.substring(6) + s.substring(3, 5) + s.substring(0, 2);
  return fromStrOp(s);
}

// \s -> n
export function fromStr (s) {
  sys.$params(arguments.length, 1);
  const r = fromStrOp(s);
  if (r.length === 0) throw new Error("Bad template '" + s + "'");
  return r[0];
}

// \s -> [n] | []
export function fromStrOp (s) {
  sys.$params(arguments.length, 1);
  if (!math.isDigits(s) || s.length != 8) return [];
  const y = s.substring(0, 4)|0;
  const m = s.substring(4, 6)|0;
  const d = s.substring(6)|0;
  return [new Date(y, m - 1, d, 12).getTime()];
}

// \n -> n
export function hour (t) {
  sys.$params(arguments.length, 1);
  return new Date(t).getHours();
}

// \n -> n
export function minute (t) {
  sys.$params(arguments.length, 1);
  return new Date(t).getMinutes();
}

// \n -> n
export function millisecond (t) {
  sys.$params(arguments.length, 1);
  return new Date(t).getMilliseconds();
}

// \n -> n
export function month (t) {
  sys.$params(arguments.length, 1);
  return new Date(t).getMonth() + 1;
}

// \n, n, n, n, n, n -> n
export function mk (day, month, year, hour, minute, second) {
  sys.$params(arguments.length, 6);
  return new Date(year, month - 1, day, hour, minute, second).getTime();
}

// \n, n, n -> n
export function mkDate (day, month, year) {
  sys.$params(arguments.length, 3);
  return new Date(year, month - 1, day, 12).getTime();
}

// \-> n
export function now () {
  sys.$params(arguments.length, 0);
  return new Date().getTime();
}

// \n -> n
export function second (t) {
  sys.$params(arguments.length, 1);
  return new Date(t).getSeconds();
}

// \n -> s
export function toEn (t) {
  sys.$params(arguments.length, 1);
  return fmt("%M-%D-%Y", t);
}

// \n -> s
export function toIso (t) {
  sys.$params(arguments.length, 1);
  return fmt("%D/%M/%Y", t);
}

// \n -> s
export function toStr (t) {
  sys.$params(arguments.length, 1);
  return fmt("%Y%M%D", t);
}

// \n -> n
export function weekday (t) {
  sys.$params(arguments.length, 1);
  return new Date(t).getDay();
}

// \n -> n
export function year (t) {
  sys.$params(arguments.length, 1);
  return new Date(t).getFullYear();
}

// \n -> n
export function yearDay (t) {
  sys.$params(arguments.length, 1);
  const dt = new Date(t);
  return Math.floor((dt - new Date(dt.getFullYear(), 0, 0)) / 86400000);
}



