// Copyright 03-May-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';

// \i, (\->*) -> <promise>*
export function delay (tm, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 0);
  return new Promise((resolve) => {
    setTimeout(() => {
      resolve(fn());
    }, tm);
  });
}

// \i -> <timer>
export function mk (tm) {
  sys.$params(arguments.length, 1);
  return [true, tm];
}

// \<timer>, (\->*) -> <promise>*
export async function run (t, fn) {
  sys.$params(arguments.length, 2);
  sys.$fparams(fn, 0);
  if (!t[0]) throw new Error('Timer has been stopped');

  async function loop () {
    const r = await delay(t[1], fn);
    if (t[0]) return loop();
    else return r;
  }

  return await loop();
}

// \<timer> -> ()
export function stop (t) {
  sys.$params(arguments.length, 1);
  if (!t[0]) throw new Error('Timer is not running');
  t[0] = false;
}

