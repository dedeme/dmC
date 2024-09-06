// Copyright 22-Jun-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';
import * as path from './path.js';
import * as bytes from './bytes.js';
import * as cryp from './cryp.js';
import {
  closeSync, copyFileSync, existsSync, lstatSync, mkdirSync, openSync,
  readdirSync, readFileSync, readSync, renameSync,
  rmSync, statSync, symlinkSync, writeFileSync, writeSync
} from "fs";
import { open } from "fs/promises";
import { pid, cwd, chdir } from "process";

// \s -> <file>
export function aopen (p) {
  sys.$params(arguments.length, 1);
  return openSync(p, "a");
}

// \s -> ()
export function cd (p) {
  sys.$params(arguments.length, 1);
  chdir(p);
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

    const tdir = path.cat([target,  path.base(source)]);
    del(tdir);
    mkdir(tdir);
    const files = dir(source);
    for (let i = 0; i < files.length; ++i) {
      const fname = files[i];
      copy(path.cat([source, fname]), tdir);
    }
    return;
  }
  if (isDirectory(target))
    target = path.cat([target, path.base(source)]);

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
  const file = await open(p);
  for await (const line of file.readLines()) {
    fn(line);
  }
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
    rpath = path.cat([dir, fpath + v.replaceAll("/", "-")]);
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


