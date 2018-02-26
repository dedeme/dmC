// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// C temporal file writer

#ifndef WRITERS_TMPC_H
  #define WRITERS_TMPC_H

#include "data.h"
#include "RW.h"

/// Writes 'fc' in tmp if there is a /*.-.*/ mark in 'fc'.
void tmpc_write(Template *tp, RW *rw);

#endif


