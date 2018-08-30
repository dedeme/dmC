// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// AST logic.
/// <p>
/// Documentacion is
/// <a href='http://localhost/mkdocs/minijsDoc/logicFunctions'>here</a>.

#ifndef SCAN_LOGIC_H
  # define SCAN_LOGIC_H

#include "dmc/Opt.h"
#include "types/ScanResult.h"
#include "types/ast.h"

/// Does sequencially a serie of scannings.
///   return: Array with all the results. Its elements can be of differents
///           types.
///   fail: If one applications of 'a' fails.
ScanResult *logic_and(AstArr *a, Scanner *sc);

/// Does sequencially a serie of scannings ([ ... | ... | ... ]).
///   return: The first successful application of 'a'.
///   fail: If every appliction of 'a' fails.
ScanResult *logic_or(AstArr *a, Scanner *sc);

/// Does a reading operation '*'
///   result: Arr, posibly empty, with values of applications of 'a'
///   fail: Never fails.
ScanResult *logic_none_or_several(AstTp *tp, Scanner *sc);

/// Does a reading operation '+'
///   result: List, with al leat one element, with values of f(context, scanner)
///   fail: If the first application of 'fn' fails.
ScanResult *logic_one_or_more(AstTp *tp, Scanner *sc);

/// Does a reading operation '?'
///   result: One Opt<value of f(context, scanner)>
///   fail: Never fails.
ScanResult *logic_one_or_none(AstTp *tp, Scanner *sc);

/// Does a reading operation '~'
///   result: opt_null().
///   fail: Application of 'tp' successes.
ScanResult *logic_not(AstMsg *tp, Scanner *sc);

#endif
