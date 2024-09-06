// Copyright 29-Jun-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';
import {realpathSync} from "fs";

function normalize(p) {
  let r = p.trim();
  while (r.endsWith("/")) r = r.substring(0, r.lenght - 1);
  return r;
}

// \s -> s
export function base (p) {
  sys.$params(arguments.length, 1);
  const r = normalize(p);
  const ix = r.lastIndexOf('/');
  if (ix !== -1) return r.substring(ix + 1);
  return r;
}

// \s -> s
export function canonical (p) {
  sys.$params(arguments.length, 1);
  return realpathSync(p);
}

// \[s.] -> s
export function cat (a) {
  sys.$params(arguments.length, 1);
  if (a.length === 0) a.push("./");
  else if (a[0] === "") a[0] = ".";
  return clean(a.join("/"));
}

// \s -> s
export function clean (p) {
  sys.$params(arguments.length, 1);
  let s = p.trim();
  if (s === "") return "";
  let bf = [];
  let isSlash = false;
  for (let i = 0; i < s.length; ++i) {
    const ch = s.charAt(i);
    if (ch === "/") {
      if (isSlash) continue;
      isSlash = true;
      bf.push(ch);
      continue;
    }
    isSlash = false;
    bf.push(ch);
  }
  s = bf.join("");
  if (s.length === 1) return s;
  if (s.endsWith("/")) s = s.substring(0, s.length - 1);

  bf = [];
  const parts = s.split("/");
  for (let i = 0; i < parts.length; ++i) {
    const part = parts[i];
    if (part === ".") continue;
    if (part === "..") {
      if (bf.length > 0) bf.pop();
      else throw new Error("Bad path for cleaning '" + p + "'");
      continue;
    }
    bf.push(part);
  }
  s = bf.join("/");
  if (s === "" && p[0] === "/")
    throw new Error("Bad path for cleaning '" + p + "'");

  return s;
}

// \s -> s
export function extension (p) {
  sys.$params(arguments.length, 1);
  const r = base(p);
  const ix = r.lastIndexOf(".");
  if (ix === -1) return "";
  return r.substring(ix);
}

// \s -> s
export function parent (p) {
  sys.$params(arguments.length, 1);
  const r = normalize(p);
  if (r === "") throw new Error("'" + p + "' has not parent directory");
  const ix = r.lastIndexOf("/");
  if (ix === -1) return "";
  if (ix === 0) return "/";
  return r.substring(0, ix);
}


