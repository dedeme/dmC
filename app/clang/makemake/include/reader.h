// Copyright 06-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef READER_H
  #define READER_H

#include "Node.h"

/// Checks directories and returns '1' if all is correct. If there is some
/// fail print a message and returns '0'.
int reader_check_directories (void);

/// Checks if for each lib 'L' it exists "lib/libL/include" and
/// "lib/libL/libL.a". It there is some fail print a message and returns '0'.
///   libs (Arr[char]): Libraries to test
int reader_check_libs (Arr *libs);

/// Adds Node1 to parent, if it proceeds, from a source line.
///   parent: Parent node
///   file: Source file (for debug). Kind "include/..." or "src/...".
///   line: Line to analize
///   nline: Line number (for debug)
///   returns:
///     - If there are some error (cyclic dependencies) prints a message and
///       returns '1'
///     - Otherwise
///       - If there are no dependency, dependency is out of "include" -
///         "src"  or it already exists, parent is not modified.
///       - Otherwise creates a new Node1 and adds it to parent-children.
int reader_dependency (Node1 *parent, char *file, char *line, int nline);

/// Adds dependencies of parent.
///   parent: Parent node
///   returns:
///     - If there are some error (cyclic dependencies) prints a message and
///       returns '1'
///     - Otherwise adds dependencies, if they exist
int reader_dependencies (Node1 *parent);

/// Returns Opt[Node1] dependencies tree.
Opt *reader_mk_tree (void);

#endif
