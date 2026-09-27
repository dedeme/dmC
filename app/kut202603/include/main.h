// Copyright 27-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Program entry.

#ifndef MAIN_H
  #define MAIN_H

#include "kut/thread.h"
#include "kut/arr.h"

///
int main(int argc, char *argv[]);

/// Returns command arguments of 'main'
///   RETURN: Arr<char>
Arr *main_args(void);

#endif
