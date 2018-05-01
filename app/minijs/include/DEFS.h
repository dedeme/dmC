// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Constants

#ifndef DEFS_H
  # define DEFS_H

///
#define VERSION "201803"

///
#define TH(tx) {txpos_printf((tx),

///
#define TH2(class, pos) {txpos_printf(\
  txpos_new(class_cpath(class), "", 0, 0, pos_nline(pos), pos_nchar(pos)),

///
#define TH3(class, pos) {txpos_printf(\
  txpos_new(final_class_cpath(class), "", 0, 0, pos_nline(pos), pos_nchar(pos)),

///
#define _TH ); THROW "\1" _THROW}

#endif
