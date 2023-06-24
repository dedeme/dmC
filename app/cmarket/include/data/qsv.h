// Copyright 05-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Quote server data.

#ifndef DATA_QSV_H
  #define DATA_QSV_H

#include "kut/tp.h"
#include "kut/rs.h"

/// Opaque structure
typedef struct qsv_Qsv Qsv;

/// Constructor
///   id: Returns Qsv identifier.
///   name: Returns Qsv name.
///   with_diary: Returns 'true' if Qsv read diary quotes.
///   download_diary: Returns the result of executing sys.cmd to read diary quotes.
///   read_diary: Returns
///                 - Tp with 2 possibly empty arrays.
///                     * Array<Kv[companyCode, value]> with diary quotes.
///                     * Array[errors] with reading errors.
///   with_historic: Returns 'true' if Qsv read historic quotes.
///   download_historic: Returns the result of executing sys.cmd to read historic quotes.
///   read_historic: Returns
///                 - Tp with
///                     * Opt<Qs> of company.
///                     * Array[errors] with reading errors.
Qsv *qsv_new (
  char *(*id)(void),
  char *(*name)(void),
  int (*with_diary)(void),
  // <char> with values: [stderr - stdout]
  Rs *(*download_diary)(void),
  // <Arr<Kv<double>>, Arr<char>>
  Tp *(*read_diary)(char *web_page),
  int (*with_historic)(void),
  // <char> with values: [stderr - stdout]
  Rs *(*download_historic)(char *co_code),
  // <Opt<Qs>, Arr<char>>
  Tp *(*read_historic)(char *web_page)
);

/// Returns this identifier.
char *qsv_id (Qsv *this);

/// Returns this name.
char *qsv_name (Qsv *this);

/// Returns 'true' if this read diary quotes.
int qsv_with_diary (Qsv *this);

/// Returns Rs<char> with values: [stderr - stdout] result of executing
/// sys.cmd to read diary quotes.
Rs *qsv_download_diary (Qsv *this);

/// Read a web page.
///   this: Quotes server.
///   web_page: Content of web page.
///   ----
///   Returns Tp<Arr<Kv<double>>, Arr<char>>
///     - Tp with
///       * Array[Kv[companyCode, quote]] with diary quotes.
///       * Array[errors] with reading errors.
///
/// NOTE: In both tuples, arrays can be empty.
Tp *qsv_read_diary (Qsv *this, char *web_page);

/// Returns 'true' if this read historic quotes.
int qsv_with_historic (Qsv *this);

/// Returns Rs<char> with values: [stderr - stdout] result of executing
/// sys.cmd to read historic quotes.
Rs *qsv_download_historic (Qsv *this, char *co_code);

/// Read a web page.
///   this: Quotes server.
///   web_page: Content of web page.
///   ----
///   Returns Tp<Opt<Qs>>, Arr<char>>
///     - Tp with
///       * Opt<Qs> of company.
///       * Array[errors] with reading errors.
Tp *qsv_read_historic (Qsv *this, char *web_page);

#endif
