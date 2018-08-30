// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// H temporal file writer

#ifndef WRITERS_TMPH_H
  #define WRITERS_TMPH_H

#include "data.h"
#include "RW.h"

/// Writes file .h in tmp if there is a /*.-.*/ mark in it.
void tmph_write(Template *tp, RW *rw);

#endif


