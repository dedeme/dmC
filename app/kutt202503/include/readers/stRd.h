// Copyright 14-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Statement reader.

#ifndef READERS_ST_RD_H
  #define READERS_ST_RD_H

#include "data/rdCtx.h"
#include "readers/expRd.h"

#define stRd_end_of_file "Unexpected end of file"

/// Record structure.
typedef struct {
  RdCtx *ctx; // Reader context.
  ExpRd *erd; // Token reader.
} StRd;

/// Constructor only to be used by 'expRd'.
///   ctx  : Reader context.
///   ExpRd: Expression reader.
StRd *stRd_new0 (RdCtx *ctx, ExpRd *erd);

/// Constructor.
///   ctx: Reader context.
///   tx : Text to read.
StRd *stRd_new (RdCtx *ctx, char *tx);

/// Returns the current statement and advances the character reader.
///   this  : Statement reader
Stat *stRd_next (StRd *this);

/// Returns the current token without avancing the character reader.
Token *stRd_peek_token (StRd *this);

/// Returns the current token and advances the character reader.
Token *stRd_next_token (StRd *this);

/// Reads definition type (left or right side).
/// NOTE: Reading starts just before the operator '(' or '['.
/// Returns Tp<Type, Exp> ([Type, Error]):
///  - Type is the read type. If there is an error it is NULL.
///  - Error is a exp_error if Type can not be read. Otherwise it is NULL.
///   this   : Statement reader.
///   is_left: 'true' if is reading the left side of a difinition.
Tp *stRd_read_def_type (StRd *this, int is_left);


#endif
