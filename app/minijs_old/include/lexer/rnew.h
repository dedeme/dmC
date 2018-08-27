// Copyright 13-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Reads a constructor (new)

#ifndef LEXER_RNEW_H
  #define LEXER_RNEW_H

typedef struct lcatt_Lcatt Lcatt;
typedef struct catt_Catt Catt;
typedef struct asym_Asym Asym;
typedef struct tx_Tx Tx;

/// 'att' is Catt 'new' with a null value.
Tx *rnew(Lcatt **instances, Catt **att, Tx *tx, Asym *symbols);

#endif
