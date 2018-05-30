// Copyright 16-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "parser/pattribute.h"
#include "Pos.h"
#include "DEFS.h"
#include "lexer/Tx.h"
#include "ast/Mtype.h"
#include "ast/Lsymbol.h"
#include "parser/pvalue.h"

Sym *pattribute(
  Class *c, char *id, Atype *generics, bool is_static, bool is_import
) {
  Aatt *atts = is_static ? class_statics(c) : class_instance(c);
  Att *a = aatt__get(atts, id);
  if (!a)
    TH2(c, pos_new(0,0))
      "%s method '%s' is missing", is_static ? "Static" : "Instance", id
    _TH

  if (is_import && !att_is_public(a))
    TH2(c, pos_new(0,0)) "'%s' is not a public attribute", id _TH

  Achar *cgenerics = class_generics(c);
  if (achar_size(cgenerics) != arr_size(generics))
    TH2(c, pos_new(0, 0))
      "'%s' is called with %d generics, but it has '%d'",
      class_id(c), arr_size(generics), achar_size(cgenerics)
    _TH
  Mtype *gen = mtype_new();
  EACH((Arr *)cgenerics, char, g) {
    mtype_put(gen, g, atype_get(generics, _i));
  }_EACH

  Achar *used = class_used(c);
  if (!achar_contains(used, id)) {
    pvalue(c, lsymbol_new(), att_type(a), att_value(a), type_child);
    achar_add(used, id);
  }

  return sym_new(type_replace(att_type(a), gen), att_is_val(a));
}
