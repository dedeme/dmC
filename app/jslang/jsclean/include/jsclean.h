// Copyright 06-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef JSCLEAN_H
  #define JSCLEAN_H

#include "dmc/std.h"
#include "data/Params.h"

///
int main (int argc, char *argv[]);

/// Opt[Params] Process 'main()' parameters. If there are some error, prints
/// it and returns 'opt_empty';
Opt *jsclean_params (int argc, char *argv[]);

#endif
