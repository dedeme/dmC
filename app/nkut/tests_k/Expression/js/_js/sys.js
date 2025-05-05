// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import { argv, env, pid as ppid, exit as pexit, stdout } from "process";
import {execFileSync} from "child_process";

export function $slice (o, begin, end) {
  if (typeof(o) === 'string') {
    const len = o.length;
    begin = begin === null ? 0 : begin < 0 ? len + begin : begin;
    begin = begin < 0 ? 0 : begin > len ? len : begin;

    end = end === null ? len : end < 0 ? len + end : end;
    end = end < begin ? begin : end > len ? len : end;
    return o.substring(begin, end);
  }
  if (Array.isArray(o)) {
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
    return Object.values(o);
  return o;
}

export function $forObject2 (o) {
  if (Object.prototype.toString.call(o) === '[object Object]')
    return Object.entries(o);
  return o.entries();
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

export function $trace (v) {
  $params(arguments.length, 1);
  const er = new Error(toStr(v));
  let msg = er.stack.split("\n")[2];
  msg = msg.substring(msg.indexOf("/") + 2, msg.lastIndexOf(":"));
  console.log(msg + ": " + er.message);
}

// Built-in --------------------

// \* -> [s.]
export function args () {
  $params(arguments.length, 0);
  return argv;
}


// \* -> b
export function asBool (e) {
  $params(arguments.length, 1);
  if (typeof(e) === 'boolean') return e;
  if (Array.isArray(e)) return e.length != 0;
  throw new Error('\nExpected: boolean or Array.\n   Found: ' + e);
}

// \* -> ()
export function assert (v) {
  $params(arguments.length, 1);
  if (!v)
    throw new Error('Assert failed');
}

// \s, [s.] -> [s, b](Result)
export function cmd (c, pars) {
  $params(arguments.length, 2);
  const [ out, err ] = cmd2(c, pars);
  return out === "" ? [err, false] : [out, true];
}

// \s, [s.] -> [s, s]
export function cmd2 (c, pars) {
  $params(arguments.length, 2);

  try {
    const stdout = execFileSync(c, pars, {
      stdio: 'pipe',
      encoding: 'utf8',
    });

    return [stdout, ""];
  } catch (err) {
    if (err.code)
      return ["", "NOEXEC: " + err.message];
    else {
      return ["", err.message];
    }
  }
}

// \ -> {s.}
export function environ () {
  $params(arguments.length, 0);
  return env;
}

// \ -> s
export function getLocale () {
  $params(arguments.length, 0);
  return env["LANG"];
}

// \n -> ()
export function exit (n) {
  $params(arguments.length, 1);
  return pexit(n);
}

// \ -> s
export function home () {
  $params(arguments.length, 0);
  return env["HOME"];
}

// \ -> s
export function pid () {
  $params(arguments.length, 0);
  return ppid;
}

// \ -> s
export function mainPath () {
  $params(arguments.length, 0);
  return argv[1];
}

// \* -> ()
export function print (v) {
  $params(arguments.length, 1);
  stdout.write(toStr(v));
}

// \* -> ()
export function printError (v) {
  $params(arguments.length, 1);
  console.error(toStr(v));
}

// \* -> ()
export function println (v) {
  $params(arguments.length, 1);
  console.log(toStr(v));
}

// \s -> ()
export function raise (v) {
  $params(arguments.length, 1);
  throw new Error(v);
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
  if (type(v) === "[object Object]") return JSON.stringify(v);
  return v.toString();
}

// \* -> s
export function type (v) {
  $params(arguments.length, 1);
  if (typeof(v) === 'object')
    return Object.prototype.toString.call(v);
  return typeof(v);
}

// \ -> s
export function user () {
  $params(arguments.length, 0);
  return env["USER"];
}


