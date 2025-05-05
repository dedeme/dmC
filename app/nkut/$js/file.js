// Copyright 22-Jun-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';
import * as bytes from './bytes.js';
import * as cryp from './cryp.js';
import {
  closeSync, copyFileSync, createReadStream, existsSync, lstatSync,
  mkdirSync, openSync, readdirSync, readFileSync, readSync, realpathSync,
  renameSync, rmSync, statSync, symlinkSync, writeFileSync, writeSync
} from "fs";
import { once } from 'events';
import { createInterface } from 'readline';
import { open } from "fs/promises";
import { pid, cwd, chdir } from "process";

// \s -> s
function normalize(p) {
  let r = p.trim();
  while (r.endsWith("/")) r = r.substring(0, r.lenght - 1);
  return r;
}

// \s -> <file>
export function aopen (p) {
  sys.$params(arguments.length, 1);
  return openSync(p, "a");
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

// \s -> ()
export function cd (p) {
  sys.$params(arguments.length, 1);
  chdir(p);
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

// \<file> -> ()
export function close (f) {
  sys.$params(arguments.length, 1);
  closeSync(f);
}

// \s, s -> ()
export function copy (source, target) {
  sys.$params(arguments.length, 2);
  if (isDirectory(source)) {
    if (!isDirectory(target))
      throw new Error("'" + target + "' is not a directory");

    const tdir = cat([target, base(source)]);
    del(tdir);
    mkdir(tdir);
    const files = dir(source);
    for (let i = 0; i < files.length; ++i) {
      const fname = files[i];
      copy(cat([source, fname]), tdir);
    }
    return;
  }
  if (isDirectory(target))
    target = cat([target, base(source)]);

  copyFileSync(source, target);
}

// \s -> ()
export function del (p) {
  sys.$params(arguments.length, 1);
  rmSync(p, { recursive: true, force: true });
}

// \s -> [s.]
export function dir (p) {
  sys.$params(arguments.length, 1);
  return readdirSync(p);
}

// \s -> b
export function exists (p) {
  sys.$params(arguments.length, 1);
  return existsSync(p);
}

// \s -> s
export function extension (p) {
  sys.$params(arguments.length, 1);
  const r = base(p);
  const ix = r.lastIndexOf(".");
  if (ix === -1) return "";
  return r.substring(ix);
}

// \s -> b
export function isDirectory (p) {
  sys.$params(arguments.length, 1);
  return exists(p) && statSync(p).isDirectory();
}

// \s -> b
export function isLink (p) {
  sys.$params(arguments.length, 1);
  return exists(p) && lstatSync(p).isSymbolicLink();
}

// \s -> b
export function isRegular (p) {
  sys.$params(arguments.length, 1);
  return exists(p) && statSync(p).isFile();
}

// \s -> ()
export function mkdir (p) {
  sys.$params(arguments.length, 1);
  mkdirSync(p, { recursive: true });
}

// \s, s -> ()
export function mklink (p, lk) {
  sys.$params(arguments.length, 2);
  symlinkSync(p, lk);
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

// \s -> s
export function read (p) {
  sys.$params(arguments.length, 1);
  return readFileSync(p, { encoding: "utf8" });
}

// \<file>, n -> <bytes>
export function readBin (f, bufLen) {
  sys.$params(arguments.length, 2);
  const bf = bytes.mk(bufLen);
  const n = readSync(f, bf, 0, bufLen, null);
  return n < bufLen ? bytes.take(bf, n) : bf;
}

// \s, \s -> () -> ()
export async function readLines (p, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);

  const rl = createInterface({
    input: createReadStream(p)
  });

  rl.on('line', (line) => {
    fn(line);
  });

  await once(rl, 'close');
}

// \[s.], \[([s]|[]).] -> () -> ()
export async function readLines2 (ps, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 1);

  const lineBfs = ps.map(() => []);
  const closes = ps.map(() => false);

  function control (nstream, isClose, line) {
    if (isClose) {
      closes[nstream] = true;
      return;
    }
    lineBfs[nstream].push(line);
    let ok = true;
    for (let i = 0; i < lineBfs.length; ++i)
      if (lineBfs[i].length === 0 && !closes[nstream]) ok = false;

    if (ok) {
      const pars = [];
      for (let i = 0; i < lineBfs.length; ++i) {
        const lb = lineBfs[i];
        if (lb.length !== 0)  pars.push([lb.shift()]);
        else pars.push([]);
      }
      fn(pars);
    }
  }

  const rls = [];
  for (let i = 0; i < ps.length; ++i) {
    const rl = createInterface({
      input: createReadStream(ps[i])
    });

    rl.on('line', (line) => control(i, false, line));
    rl.on('close', () => control(i, true, ""));

    rls.push(rl);
  }

  for (let i = 0; i < rls.length; ++i)
    await once(rls[i], 'close');
}

// \s, s -> ()
export function rename (oldPath, newPath) {
  sys.$params(arguments.length, 2);
  renameSync(oldPath, newPath);
}

// \s -> <file>
export function ropen (p) {
  sys.$params(arguments.length, 1);
  return openSync(p, "r");
}

// \s -> n
export function size (p) {
  sys.$params(arguments.length, 1);
  return statSync(p).size;
}

// \s -> n
export function tm (p) {
  sys.$params(arguments.length, 1);
  return statSync(p).mtime;
}

// \s, s -> s
export function tmp (dir, fpath) {
  sys.$params(arguments.length, 2);
  if (dir == "") dir = "/tmp";
  let rpath = "";
  while (true) {
    const k = cryp.genK(16);
    const v = cryp.encode(k, "" + pid).substring(0, 8);
    rpath = cat([dir, fpath + v.replaceAll("/", "-")]);
    if (!exists(fpath)) break;
  }
  return rpath;
}

// \ -> s
export function wd () {
  sys.$params(arguments.length, 0);
  return cwd();
}

// \s -> <file>
export function wopen (p) {
  sys.$params(arguments.length, 1);
  return openSync(p, "w");
}

// \s, s -> ()
export function write (p, text) {
  sys.$params(arguments.length, 2);
  writeFileSync(p, text);
}

// \<file>, <bytes> -> ()
export function writeBin (f, data) {
  sys.$params(arguments.length, 2);
  writeSync(f, data);
}

// \<file>, s -> ()
export function writeText (f, text) {
  sys.$params(arguments.length, 2);
  writeSync(f, text);
}

