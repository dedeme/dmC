// Copyright 14-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tuple of 5 elements.

#ifndef DATA_TP5_H
  #define DATA_TP5_H

/// Structure.
typedef struct {
  void *e1;
  void *e2;
  void *e3;
  void *e4;
  void *e5;
} Tp5;

/// Constructor.
Tp5 *tp5_mk (void *e1, void *e2, void *e3, void *e4, void *e5);

#endif
