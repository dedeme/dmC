// Copyright 05-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';
import * as iter from './iter.js';

// \s -> ()
export function del (s) {
  sys.$params(arguments.length, 1);
  localStorage.removeItem(s);
}

// \s -> ()
export function get (s) {
  sys.$params(arguments.length, 1);
  const r = localStorage.getItem(s);
  return r == null ? [] : [r];
}

// \-> <iter>[s...]
export function keys () {
  sys.$params(arguments.length, 0);
  let i = 0;
  return iter.mk(
    () => i < localStorage.length,
    () => localStorage.key(i++)
  );
}

// \s, s -> ()
export function put (key, value) {
  sys.$params(arguments.length, 2);
  localStorage.setItem(key, value);
}


