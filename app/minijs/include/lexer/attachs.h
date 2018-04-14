// Copyright 06-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// attachments reader.

#ifndef LEXER_ATTACHS_H
  #define LEXER_ATTACHS_H

#include <dmc/all.h>
#include "ast/Attach.h"
#include "lexer/Txpos.h"

/// Read an Attach dot and adds it to 'atts'.
Txpos *attachs_read_dot(Arr/*Attach*/ *atts, Txpos *tx);

/// Read an Attach dot or sub and adds it to 'atts'.
Txpos *attachs_read_dot_sub(Arr/*Attach*/ *atts, Txpos *tx);

/// Read an Attach dot, sub or fn and adds it to 'atts'.
Txpos *attachs_read_all(Arr/*Attach*/ *atts, Txpos *tx);

#endif
