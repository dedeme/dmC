// Copyright 17-Apr-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Documentation entry.
/// Entries take the fallowing scheme:
///     Summary
///       s id code line doc
///     Imports
///       i id code line doc
///     Types
///       t id code line doc
///     Values
///       v id code line doc
///     Enumerations
///       e id code line doc (id is (1), (2), (3) ...)
///     Indexeds
///       x id code line doc
///     Functions
///       f id code line doc
///     Error
///       r idEmpty codeEmtpy line error

#ifndef DATA_DOC_E_H
  #define DATA_DOC_E_H

#include "kut/arr.h"
#include "kut/opt.h"
#include "data/type.h"

/// Record structure.
typedef struct {
  char type; // Type: one of s, i, t, v, e, x, f or r
  char *id; // Identifier. (Empty in summary and errors)
  char *code; // Code. (Empty in summary and errors)
  int ln; // Number of line.
  char *doc; // Documentation, buf for errors where it is the error message.
} DocE;

/// Constructor
DocE *docE_new_summary (char *text);

/// Constructor
DocE *docE_new_import (char *id, char *code, int ln, char *link);

/// Constructor
DocE *docE_new_type (char *id, char *code, int ln, char *doc);

/// Constructor
DocE *docE_new_value (char *id, char *code, int ln, char *doc);

/// Constructor
DocE *docE_new_enum (char *id, char *code, int ln, char *doc);

/// Constructor
DocE *docE_new_indexed (char *id, char *code, int ln, char *doc);

/// Constructor
DocE *docE_new_function (char *id, char *code, int ln, char *doc);

/// Constructor
DocE *docE_new_error (int ln, char *message);

/// JSON serializacion (ex. ["v", "foo", "foo = 3", 23, "Some foo"])
char *docE_to_js (DocE *this);

#endif

