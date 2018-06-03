// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Parameter reader.<br>
/// Definition:
///   [+|-]id: type [: fserial]
/// <tt>[+|-]id</tt>
///   '+' -> With getter and setter
///   '-' -> Without getter or setter
///   default -> With getter but without setter
/// <tt>: fserial</tt>
///   '_bool'      -> Serialize with jarr_abool() and restore with jarr_gbool()
///   '_int'       -> Serialize with jarr_aint() and restore with jarr_gint()
///   '_uint'      -> Serialize with jarr_auint() and restore with jarr_guint()
///   '_string'    -> Serialize with jarr_astring() and restore with
///                   jarr_gstring()
///   '_sarray'    -> Serialize wint jarr_aarray() using 'json_wstring()' as
///                   function 'to()' and 'json_rstring()' as function 'from()'
///   '_sobject'   -> Serialize wint jarr_aobject() using 'json_wstring()' as
///                   function 'to()' and 'json_rstring()' as function 'from()'
///   '_double n'  -> Serialize with jarr_adouble() using 'n' decimals number
///                   and restore with jarr_gdouble()
///   'fn'         -> Serialize with 'fn_serialize()' and restore with
///                   'fn_restore()'
///   '_array fn'  -> Serialize with jarr_aarray() using 'fn_serialize()' as
///                   function 'to()' and 'fn_restore()' as function 'from()'
///   '_object fn' -> Serialize with jarr_aobject() using 'fn_serialize' as
///                   function 'to()' and 'fn_restore()' as function 'from()'


#ifndef READERS_PARAM_H
  #define READERS_PARAM_H

#include "data.h"
#include "RW.h"

///
Param *param_read(RW *rw, Head *head);

#endif
