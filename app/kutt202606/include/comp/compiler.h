// Copyright 22-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Compiler.

#ifndef COMP_COMPILER_H
  #define COMP_COMPILER_H

/// Compiles 'modules' and generarate '.o' files.
/// if 'only_compile' is FALSE, it also generates '.exe' file.
/// Returns empty string or a message if there was some error.
char *compiler_run (int only_compile);

#endif
