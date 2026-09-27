// Copyright 05-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Quotes reader.

#ifndef CQUOTES_H
  #define CQUOTES_H

#include "exp.h"

///
typedef struct {
  // Companies number.
  int ncos;
  // Company nicks.
  char **cos;
  // Quotes dates in format YYYYMMDD, from before to after.
  char **dates;
  //  Array of normalized open quotes (without -1).
  //  Its rows match 'dates' and its columns 'cos'.
  double **opens;
  //  Array of normalized close quotes (without -1).
  //  Its rows match 'dates' and its columns 'cos'.
  double **closes;
  //  Array of normalized maximum quotes (without -1).
  //  Its rows match 'dates' and its columns 'cos'.
  double **maxs;
  //  Array of normalized minimum quotes (without -1).
  //  Its rows match 'dates' and its columns 'cos'.
  double **mins;
} Quotes;

/// Returns an empty Quotes.
///   n_cos: Number of companies.
Quotes *quotes_new (int n_cos);

/// Reads quotes.
///   dpath : Directory with files 'NICK'.tb.
///   ncos  : Number of companies to read.
///   cos   : Nicks of companies to read.
Quotes *cquotes_read (char *dpath, int ncos, char **cos);

/// Reads quotes.
/// Creates an opaque C object reading from 'dpath'.
/// Returns Exp<<cquotes>>
///   params: Arr<s, [s.]>. Fields are:
///             dpath : Directory with files 'NICK'.tb.
///             cos   : Nicks of companies to read.
Exp *libmkt_cquotes_read (Arr *params);

/// Returns the index of one company from its nick, or -1 if nick is not found.
/// Returns Exp<i>
///   params: Arr<<cquotes>, s>. Fields are:
///             cqts: C quotes.
///             nick: Company nick.
Exp *libmkt_cquotes_company_index (Arr *params);

/// Extracts data of the company with index 'coIx' to be used in 'strategy'.
/// Returns Exp<<cquotes>>
///   params: Arr<<cquotes>, s>. Fields are:
///             cqts: C quotes.
///             co_ix: Index in cqts of the company to extract.
Exp *libmkt_cquotes_get_single (Arr *params);

#endif
