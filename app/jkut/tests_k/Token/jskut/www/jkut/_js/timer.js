// Copyright 03-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';

// \i, (\->()) -> <timer>
export function delay (tm, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 0);
  const t = mk(tm);
  run(t, () => {
    fn();
    stop(t);
  });
}

// \i -> <timer>
export function mk (tm) {
  sys.$params(arguments.length, 1);
  return [null, tm];
}

// \<timer>, (\->()) -> ()
export function run (t, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 0);
  if (t[0] != null) throw new Error('Timer is already run');
  t[0] = setInterval(fn, t[1]);
}

// \<timer> -> ()
export function stop (t) {
  sys.$params(arguments.length, 1);
  if (t[0] == null) throw new Error('Timer is not running');
  clearInterval(t[0]);
  t.length = 0;
}

