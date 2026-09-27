// Copyright 14-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Image widget.

#include "exp.h"

#ifndef IMAGE_H
  #define IMAGE_H

/// Creates a new image.
///   * If 'params' has size 1, 'name' is a file path.
///   * If 'params' has size 2, 'name' is a icon-theme name.
///   name: File path or icon-theme name. If its a file name:
///         - if 'name' does not contains dot ('.'), '.png' is added.
///         - if 'name' does not contains slash ('/'), 'img/' is prepended.
///   size: (if there are 2 parameters) Icon size.
/// \s -> <gwg> or
/// \s, i -> <gwg>
Exp *libkgtk_label_new (Arr *params);

#endif
