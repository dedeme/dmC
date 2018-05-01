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
#define TH2(class, pos) {txpos_printf(\
  txpos_new(class_cpath(class), "", 0, 0, pos_nline(pos), pos_nchar(pos)),

///
#define TH3(class, pos) {txpos_printf(\
  txpos_new(final_class_cpath(class), "", 0, 0, pos_nline(pos), pos_nchar(pos)),

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
  VLUNARY,
  VRUNARY,
  VBINARY,
  VTERNARY,
  VWITH,
  VGROUP,
  VNEW
};

/// Identifier type
enum Attach_t {
  ADOT,
  ASUB,
  AFN
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
  SINC,
  SASSIGN,
  SRETURN,
  SBREAK,
  SCONTINUE,
  SBLOCK,
  SWHILE,
  SDO,
  SFOR0,
  SFOR,
  SFOR_EACH,
  SIF,
  STRY,
  STHROW,
  SNATIVE
};

/// Cvalue types
enum Cvalue_t {
  VAL,
  VAR
};

#endif
