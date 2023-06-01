// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

export function $slice (o, begin, end) {
  if (typeof(o) === 'string') {
    const len = o.length;
    begin = begin === null ? 0 : begin < 0 ? len + begin : begin;
    begin = begin < 0 ? 0 : begin > len ? len : begin;

    end = end === null ? len : end < 0 ? len + end : end;
    end = end < begin ? begin : end > len ? len : end;
    return o.substring(begin, end);
  }
  if (typeof(o) === 'object' && Array.isArray(o)) {
    if (begin === null) begin = 0;
    if (end === null) end = o.length;
    return o.slice(begin, end);
  }

  throw new Error('\nExpected: Array or string.\n   Found: ' + o);
}

export function $eq (e1, e2) {
  if (typeof(e1) !== typeof(e2)) return false;
  if (typeof(e1) === 'object') {
    if (Array.isArray(e1)) {
      if (Array.isArray(e2)) {
        if (e1.length !== e2.length) return false;
        for (let i = 0; i < e1.length; i++)
          if (!$eq(e1[i], e2[i])) return false;
        return true;
      }
      return false;
    }
    if (Array.isArray(e2)) return false;
    if (Object.prototype.toString.call(e1) === '[object Object]') {
      if (Object.prototype.toString.call(e2) === '[object Object]') {
        const keys1 = Object.keys(e1);
        const keys2 = Object.keys(e2);
        if (keys1.length !== keys2.length) return false;
        for (let i = 0; i < keys1.length; i++) {
          const k = keys1[i];
          if (!$eq(e1[k], e2[k])) return false;
        }
        return true;
      }
      return false;
    }
    if (Object.prototype.toString.call(e2) === '[object Object]') return false;
  }
  if (typeof(e1) === 'number' || typeof(e2) === 'number')
    return e1 + 0.0000001 >= e2 && e1 - 0.0000001 <= e2;
  return e1 === e2;
}

export function $neq (e1, e2) {
  return !$eq(e1, e2);
}

export function $forObject (o) {
  if (Object.prototype.toString.call(o) === '[object Object]')
    return Object.keys(o);
  return o;
}

export function $forCmp (v) {
  if (v === 0) throw new Error('Step of for loop is 0');
  if (v > 0) return (v, l) => (v <= l);
  return (v, l) => (v >= l);
}

export function $params (actual, expected) {
  if (actual != expected)
    throw new Error(
      'Bad number of parameters' +
      '\n  Expected: ' + expected +
      '\n    Actual: ' + actual
    );
}

export function $fparams (fn, expected) {
  const actual = fn.length;
  if (actual != expected)
    throw new Error(
      'Bad number of parameters in ' + fn +
      '\n  Expected: ' + expected +
      '\n    Actual: ' + actual
    );
}

export function $null (v) {
  if (v == null) return [];
  return [v];
}

export function $checkExists (left, right) {
  if (left == null) throw new Error('Index out of range or key not found');
  return right;
}

export function $checkNull (v) {
  if (typeof(v) === "number" && !Number.isFinite(v))
    throw new Error ('Bad Number ' + v);
  if (v == null) throw new Error('Null expression ' + v);
  return v;
}

// Built-in --------------------

// \* -> b
export function asBool (e) {
  $params(arguments.length, 1);
  if (typeof(e) === 'boolean') return e;
  if (typeof(e) === 'string') return e != '';
  if (typeof(e) === 'object' && Array.isArray(e)) return e.length != 0;
  throw new Error('\nExpected: boolean, string or Array.\n   Found: ' + e);
}

// \* -> ()
export function assert (v) {
  $params(arguments.length, 1);
  if (!asBool(v))
    throw new Error('Assert failed');
}

// \*, * -> ()
export function test (actual, expected) {
  $params(arguments.length, 2);
  if ($neq(actual, expected))
    throw new Error(
      'Test failed:\nExpected: ' + JSON.stringify(expected) +
      '\n  Actual: ' + JSON.stringify(actual)
    );
}

// \* -> s
export function toStr (v) {
  $params(arguments.length, 1);
  if (v == null) return "<null>";
  if (typeof(v) === 'object') return JSON.stringify(v);
  return v.toString();
}

// \* -> s
export function type (v) {
  $params(arguments.length, 1);
  if (typeof(v) === 'object')
    return Object.prototype.toString.call(v);
  return typeof(v);
}

