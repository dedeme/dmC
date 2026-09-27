// Copyright 25-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Scrolled window widget.

#include "exp.h"

#ifndef SCROLLED_WINDOW_H
  #define SCROLLED_WINDOW_H

/// Creates a new scrolled window.
/// \ -> <gwg>
Exp *libkgtk_scrolled_window_new (Arr *params);

/// Sets 'w' child.
///   w: Scrolled window.
///   child: Widget to embed.
/// \<wg>, <wg> -> <wg>
Exp *libkgtk_scrolled_window_add (Arr *params);

#endif
