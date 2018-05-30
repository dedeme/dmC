// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code Opt template.<p>
/// Use:
///   ///
///   typedef struct ochar_Ochar Ochar;
///   #define TY char                    // Element type
///   #define CT Ochar                   // Container type
///   #define FUN(id) ochar ## _ ## id   // Defintion for functions
///   #include "dmc/tpl/topt.c"
///   #undef TY
///   #undef CT
///   #undef FUN


#include "dmc/Opt.h"

inline
CT *FUN(null)() {
  return (CT *)opt_null();
}

inline
CT *FUN(new)(TY *v) {
  return (CT *)opt_new(v);
}

inline
bool FUN(is_null)(CT *this) {
  return opt_is_null((Opt *)this);
}

inline
TY *FUN(value)(CT *this) {
  return (TY *)opt_value((Opt *)this);
}
