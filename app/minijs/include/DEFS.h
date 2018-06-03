// Copyright 03-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Constants

#ifndef DEFS_H
  # define DEFS_H

///
#define VERSION "201806"

///
#define TH(tx) {tx_printf((tx),

///
#define TH1(path, pos) \
  TH(tx_new((path), "", "", pos_nline(pos), pos_nchar(pos)))

///
#define TH2(class, pos) TH1(cpath_fpath(cpath_from_id(class_id(class))), pos)

///
#define _TH ); THROW "\1" _THROW}

#endif
