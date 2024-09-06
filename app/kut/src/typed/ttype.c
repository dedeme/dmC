// Copyright 22-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "typed/ttype.h"
#include "kut/buf.h"

Ttype ttype_from_sym (char sym) {
  switch (sym) {
    case 'b': return ttype_BOOL;
    case 'i': return ttype_INT;
    case 'f': return ttype_FLOAT;
    case 's': return ttype_STRING;
    case 'o': return ttype_OBJECT;
    case 'I': return ttype_AINT;
    case 'F': return ttype_AFLOAT;
    case 'S': return ttype_ASTRING;
    case 'D': return ttype_DIC;
    case '0': return ttype_NO_RETURN;
    default: return ttype_ERROR;
  }
}

char *ttype_to_str (Ttype type) {
  if (type < 0 || type >= ttype_ERROR) return "?";
  return str_new_c("bifsoIFSD0"[type]);
}

char *ttype_group_to_str (Iarr *types) {
  Buf *bf = buf_new();
  EACHI(types, tp) {
    buf_add(bf, ttype_to_str(tp));
  }_EACH
  return buf_str(bf);
}

char *ttype_to_c (Ttype t) {
  switch (t) {
    case ttype_BOOL: return "int";
    case ttype_INT: return "int64_t";
    case ttype_FLOAT: return "double";
    case ttype_STRING: return "char *";
    case ttype_OBJECT: return "void *";
    case ttype_AINT: return "Arri *";
    case ttype_AFLOAT: return "Arrf *";
    case ttype_ASTRING: return "Arrs *";
    case ttype_DIC: return "Arrs *";
    case ttype_NO_RETURN: return "";
    case ttype_ERROR : return "??";
  }
  return "??";
}

char *ttype_to_func (Iarr *tpars, Ttype tret) {
  char *ret = ttype_to_str(tret);
  return str_f("%s|%s", ttype_group_to_str(tpars), *ret == '0' ? "" : ret);
}
