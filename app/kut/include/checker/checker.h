// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code checker.

#ifndef CHECKER_CHECKER_H
  #define CHECKER_CHECKER_H

#include "module.h"

/// Check a module. This function must be called after reading every module.
///   fix: Number module-file.
///   md : Module to check.
void checker_run (int fix, Module *md);

#endif
