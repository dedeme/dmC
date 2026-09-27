// Copyright 10-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Label widget.

#include "exp.h"

#ifndef LABEL_H
  #define LABEL_H

/// Creates a new label.
/// \ -> <gwg>
Exp *libkgtk_label_new (Arr *params);

/// Sets 'w' text.
///   w : Widged to set.
///   tx: Value.
/// \<wg>, s -> <wg>
Exp *libkgtk_label_text (Arr *params);

/// Sets 'w' text in html format.
///   w : Widged to set.
///   tx: Value.
/// \<wg>, s -> <wg>
Exp *libkgtk_label_html (Arr *params);

/// Sets 'w' selectable ('true').
///   w    : Widged to set.
///   value: Selectable value.
/// \<wg>, b -> <wg>
Exp *libkgtk_label_selectable (Arr *params);


#endif
