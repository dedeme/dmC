// Copyright 29-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Skeleton syntax functions.
/// <p>
/// Documentacion is
/// <a href='http://localhost/mkdocs/minijsDoc/skSyntaxFunctions'>here</a>.

#ifndef SCAN_SKSYNTAXFNS_H
  # define SCAN_SKSYNTAXFNS_H

#include "types/SkeletonContext.h"
#include "types/Scanner.h"
#include "types/ScanResult.h"

/// Scans 'SkTopDeclaration', a top skeleton declaration.
ScanResult *sk_syntax_fns_SkTopDeclaration(
  SkeletonContext *context, Scanner *sc
);


#endif

