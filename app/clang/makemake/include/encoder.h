// Copyright 07-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef ENCODER_H
  #define ENCODER_H

#include "Node.h"

/// Generates 'mk' script. 'ns' is an Arr[Node2] sorted with 'node2_sort'.
char *encoder_mk (Arr *ns);

/// Generates a 'Makefile' script.
///   libs: Arr[char] Libraries names (e.g. "dmc")
///   n: Node from make makefile
char *encoder_makefile (Arr *libs, Node2 *n);

/// Generates main 'Makefile' script
///   prg: Program name
///   libs: Arr[char] Libraries names (e.g. "dmc")
///   ns: Arr[Node1] All nodes
///   n: Main node from make makefile
char *encoder_main_makefile (char *prg, Arr *libs, Arr *ns, Node2 *n);

#endif
