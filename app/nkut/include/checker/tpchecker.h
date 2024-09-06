// Copyright 25-Jul-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Type checker.

#ifndef CHECKER_TPCHECKER_H
  #define CHECKER_TPCHECKER_H

/// Check types of module. This function must be called after reading every
/// module.
///   fix: Number module-file.
void tpchecker_run (int fix);

#endif
