// Copyright 27-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Two layers widget.

#include "exp.h"

#ifndef OVERLAY_H
  #define OVERLAY_H

/// Creates a new overlay widget.
/// \ -> <gwg>
Exp *libkgtk_overlay_new (Arr *params);

/// Sets the background widget of 'w'.
///   w   : Overlay widget to set.
///   back: Backgroun widget
/// \<wg>, <wg> -> <wg>
Exp *libkgtk_overlay_set (Arr *params);

/// Sets the foreground widget of 'w'.
///   w   : Overlay widget to set.
///   fore: Foreground widget
/// \<wg>, <wg> -> <wg>
Exp *libkgtk_overlay_add (Arr *params);

/// Removes the foreground widget of 'w'.
///   w   : Overlay widget to set.
///   fore: Foreground widget
/// \<wg>, <wg> -> <wg>
Exp *libkgtk_overlay_remove (Arr *params);

#endif
