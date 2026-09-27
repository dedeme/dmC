// Copyright 15-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Entry widget.

#include "exp.h"

#ifndef ENTRY_H
  #define ENTRY_H

/// Creates a new entry.
/// \ -> <gwg>
Exp *libkgtk_entry_new (Arr *params);

/// Sets 'w' text.
///   w  : Widged to set.
///   val: Value.
/// \<wg>, s -> <wg>
Exp *libkgtk_entry_text (Arr *params);

/// Returns Exp<s> with text of 'w'.
///   w : Widget to read.
/// \<wg> -> s
Exp *libkgtk_entry_get_text (Arr *params);

/// Adds a button to 'w'.
///   w      : Widget to set.
///   img    : Widget of type 'image' to add.
///   isLeft : 'true' if 'img' is added at left. 'false' if 'img' is added at
///            right.
///   tooltip: Tooltip text. If its value is "", no tooltip will be added.
/// \<wg>, <wg>, b, s -> <wg>
Exp *libkgtk_entry_icon (Arr *params);

/// Grabs focus without selecting.
///   w : Widget to read.
/// \<wg> -> <wg>
Exp *libkgtk_entry_focus (Arr *params);

#endif
