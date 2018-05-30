// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Header Opt template.<p>
/// Use:
///   ///
///   typedef struct ochar_Ochar Ochar;
///   #define TY char                    // Element type
///   #define CT Ochar                   // Container type
///   #define FUN(id) ochar ## _ ## id   // Defintion for functions
///   #include "dmc/tpl/topt.h"
///   #undef TY
///   #undef CT
///   #undef FUN

#include "dmc/Opt.h"

CT *FUN(null)();

/// opt_new throws exception if 'v' is null.
/// Throws exception
CT *FUN(new)(TY *v);

///
bool FUN(is_null)(CT *this);

/// opt_value trhows exception if 'this' value is null.
/// Throws exception.
TY *FUN(value)(CT *this);
