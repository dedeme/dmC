// Copyright 30-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Statements reader

#ifndef LEXER_STATS_H
  #define LEXER_STATS_H

#include <dmc/all.h>
#include "ast/Stat.h"
#include "lexer/Txpos.h"

/// stats_read read a statement. If fails throws a tx_exception.
Txpos *stats_read(Stat **stat, Txpos *tx);

/// stats_block reads an array of statements. If fails throws a tx_exception.
Txpos *stats_block(Arr/*Stat*/ **stats, Txpos *tx);

#endif
