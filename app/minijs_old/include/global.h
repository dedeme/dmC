// Copyright 12-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Global containers

#ifndef GLOBAL_H
  # define GLOBAL_H

#include <dmc/all.h>

///
void global_init();

/// global_roots return source paths.
Arr/*char*/ *global_roots();

/// global_final_classes return classes which are totally or partially
/// implemented and written.
Arr/*Final_class*/ *global_final_classes();

/// global_classes return classes which have been read by lexer
Arr/*Class*/ *global_classes();

#endif

