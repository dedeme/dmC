// Copyright 06-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Volumes reader.

#ifndef VOLUMES_H
  #define VOLUMES_H

#include "exp.h"

/// Reads volume average (in €) ([f.]) of serveral companies.
/// Returns one value for each company. If data can not be read, the company
/// value is set to 0.
///   params: (Arr<s, i, [s.]>). Fields are:
///             dpath  : Directory with files 'NICK'.tb.
///             samples: Number of samples to make the average.
///             cos    : Nicks of companies to read.
Exp *libmkt_volumes_read (Arr *params);

#endif
