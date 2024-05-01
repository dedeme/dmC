// Copyright 28-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

import * as sys from './sys.js';

// \s -> *
export function r (json) {
  sys.$params(arguments.length, 1);
  return JSON.parse(json);
}

// \* -> s
export function w (e) {
  sys.$params(arguments.length, 1);
  return JSON.stringify(e);
}


