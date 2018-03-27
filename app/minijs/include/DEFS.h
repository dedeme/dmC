// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Constants

#ifndef DEFS_H
  # define DEFS_H

#define VERSION "201803"

#define TH(tx) {txpos_printf((tx),

#define _TH ); THROW "\1" _THROW}

enum Vtype_t {
  VNULL,
  VBOOL,
  VBYTE,
  VINT,
  VFLOAT,
  VCHAR,
  VSTR,
  VARR,
  VMAP,
  VFN,
  VID,
  VFID,
  VLMONADIC,
  VRMONADIC,
  VBINARY,
  VTERNARY
};

enum Type_t {
  DATA,
  ARR,
  MAP,
  FN,
  ANY,
  UNKNOWN
};

enum Stat_t {
  DECLARATION
};

enum Cvalue_t {
  VAL,
  VAR,
  METHOD,
  GETTER,
  SETTER
};


#endif
