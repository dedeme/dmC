// Copyright 06-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Reads Class attributes

#ifndef LEXER_RCATTS_H
  #define LEXER_RCATTS_H

typedef struct lcatt_Lcatt Lcatt;
typedef struct asym_Asym Asym;
typedef struct tx_Tx Tx;

/// New attributes are adds to 'atts'.
Tx *rcatts(
  Lcatt **atts, Tx *tx, bool is_public, bool is_static, Asym *symbols
);

#endif
