// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Expression reader.

#ifndef READERS_EXP_RD_H
  #define READERS_EXP_RD_H

#include "data/rdCtx.h"
#include "data/exp.h"
#include "data/token.h"
#include "data/tp.h"
#include "readers/tokenRd.h"

/// Record structure.
typedef struct {
  RdCtx *ctx;    // Reader context.
  TokenRd *tkrd; // Token reader.
} ExpRd;

/// Constructor.
///   ctx: Reader context.
///   tx : Text to read.
ExpRd *expRd_new (RdCtx *ctx, char *tx);

/// Returns the current no-empty expression and advances the character reader.
Exp *expRd_next (ExpRd *this);

/// Returns the current token and advances the character reader.
Token *expRd_next_token (ExpRd *this);

/// Returns the current token without avancing the character reader.
Token *expRd_peek_token (ExpRd *this);

/// Returns Tp<Arr<Exp>, Exp> ([Array, Error]):
///  - Array is an array of expressions. If there is an error it is empty.
///  - Error is a exp_error if Array is empty. Otherwise it is NULL.
///   this: Expression reader.
///   end : ']' for array and tuple, ')' for parenthesis and ':' for switch.
Tp *expRd_read_array (ExpRd *this, char end);

/// Reads definition type (left or right side).
/// NOTE: Reading starts just before the operator '(' or '['.
/// Returns Tp<Type, Exp> ([Type, Error]):
///  - Type is the read type. If there is an error it is NULL.
///  - Error is a exp_error if Type can not be read. Otherwise it is NULL.
///   this   : Expression reader.
///   is_left: 'true' if is reading the left side of a difinition.
Tp *expRd_read_def_type (ExpRd *this, int is_left);

/// Reads the type of empty dictionary or array.
/// NOTE: Reading starts just before the operator '/'.
/// Returns Tp<Type, Exp> ([Type, Error]):
///  - Type is the read type. If there is an error it is NULL.
///  - Error is a exp_error if Type can not be read. Otherwise it is NULL.
///   this: Expression reader.
Tp *expRd_read_empty_type (ExpRd *this);

#endif
