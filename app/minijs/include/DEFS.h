// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Constants

#ifndef DEFS_H
  # define DEFS_H

///
#define VERSION "201803"

///
#define TH(tx) {txpos_printf((tx),

///
#define _TH ); THROW "\1" _THROW}

/// Import types
enum Itype_t {
  ISINGLE,
  IID,
  IADDS,
  ISUBS
};

/// Value types
enum Vtype_t {
  VNULL,
  VBOOL,
  VBYTE,
  VINT,
  VFLOAT,
  VCHAR,
  VSTR,
  VSTR2,
  VARR,
  VMAP,
  VFN,
  VID,
  VFID,
  VLUNARY,
  VRUNARY,
  VBINARY,
  VTERNARY,
  VWITH,
  VGROUP
};

/// Type types
enum Type_t {
  DATA,
  ARR,
  MAP,
  FN,
  ANY,
  UNKNOWN
};

/// Statement types
enum Stat_t {
  SVAL,
  SVAR,
  SFN,
  SRETURN
};

/// Cvalue types
enum Cvalue_t {
  VAL,
  VAR,
  METHOD
};

#endif
