// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tuple of 4 elements.

#ifndef DATA_TP4_H
  #define DATA_TP4_H

/// Structure.
typedef struct {
  void *e1;
  void *e2;
  void *e3;
  void *e4;
} Tp4;

/// Constructor.
Tp4 *tp4_mk (void *e1, void *e2, void *e3, void *e4);

#endif
