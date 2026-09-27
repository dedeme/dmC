// Copyright 15-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Password widget.

#include "exp.h"

#ifndef PASWORD_H
  #define PASWORD_H

/// Creates a new password widget.
/// \ -> <gwg>
Exp *libkgtk_password_new (Arr *params);

/// Adds or removes an icon to 'w'.
///   w   : Password to set.
///   show: Show icon if 'true' or removes it if 'false'.
/// \<wg>, b -> <wg>
Exp *libkgtk_password_with_icon (Arr *params);

/// Sets 'w' text.
///   w  : Widged to set.
///   val: Value.
/// \<wg>, s -> <wg>
Exp *libkgtk_password_text (Arr *params);

/// Returns Exp<s> with text of 'w'.
///   w : Widget to read.
/// \<wg> -> s
Exp *libkgtk_password_get_text (Arr *params);

#endif
