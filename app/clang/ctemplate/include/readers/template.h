// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Template reader.<br>
/// Definition:
///   /*[.+.]
///   STRUCTRURE+
///   */

#ifndef READERS_TEMPLATE_H
  #define READERS_TEMPLATE_H

#include "data.h"
#include "RW.h"

/// Reads a Template from 'fc'. If 'fc' has not template returns NULL;
Template *template_read(RW *rw);

#endif

