// Copyright 22-Jun-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as timer from './timer.js';
import * as sys from './sys.js';

// \[n], n -> b
export function sleep (startV, delay) {
  sys.$params(arguments.length, 2);
  let now = new Date().getTime();
  if (now - startV[0] > delay) {
    startV[0] = now;
    return false;
  }
  return true;
}

// \\[b], <timer> -> () -> [b]
export function run (fn) {
  sys.$params(arguments.length, 1);
  sys.$fparams(fn, 2);
  const endV = [false];
  const tm = [true, 1];
  timer.run(tm, () => fn(endV, tm));
  return endV;
}

