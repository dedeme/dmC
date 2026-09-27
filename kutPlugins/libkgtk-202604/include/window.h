// Copyright 10-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Window widget.

#include "exp.h"

#ifndef WINDOW_H
  #define WINDOW_H

/// Creates a new window.
/// \<gtkApplication> -> <gwg>
Exp *libkgtk_window_new (Arr *params);

/// Adds 'child' to 'w'.
///   w    : Widged to set.
///   child: Widget to add.
/// \<wg>, <wg> -> <wg>
Exp *libkgtk_window_add (Arr *params);

/// Closes 'w'.
///   w    : Widged to set.
/// \<wg> -> <wg>
Exp *libkgtk_window_close (Arr *params);

/// Returns the application of 'w'.
///   w    : windows.
/// \<wg> -> <gtkApplication>
Exp *libkgtk_window_get_app (Arr *params);

/// Maximizes or unmaximizes 'w'.
///   w    : Widged to set.
///   value: 'true' maximizes and 'false' unmaximizes.
/// \<wg>, b -> <wg>
Exp *libkgtk_window_maximize (Arr *params);

/// Minimizes or unminimizes 'w'.
///   w    : Widged to set.
///   value: 'true' minimizes and 'false' unminimizes.
/// \<wg>, b -> <wg>
Exp *libkgtk_window_minimize (Arr *params);

/// Sets or unsets 'w' as modal window.
///   w    :  Window to set.
///   parent: Parent window.
/// \<wg>, <wg> -> <wg>
Exp *libkgtk_window_modal (Arr *params);

/// Show 'w'.
///   w: Widged to show.
/// \<gwg> -> ()
Exp *libkgtk_window_present (Arr *params);

/// Set if 'w' draws a rectangle in the focused widget.
///   w  : Window to set,
///   value: 'true' to show the rectangle, 'false to hidden it.
Exp *libkgtk_window_focus_visible (Arr *params);

/// Sets 'w' size.
///   w    : Widged to set.
///   width: Value.
///   height: Value.
/// \<wg>, i, i -> ()
Exp *libkgtk_window_size (Arr *params);

/// Sets 'w' text (label).
///   w : Widged to set.
///   tx: Value.
/// \<wg>, s -> <wg>
Exp *libkgtk_window_text (Arr *params);

#endif
