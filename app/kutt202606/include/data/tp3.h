// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tuple of 3 elements.

#ifndef DATA_TP3_H
  #define DATA_TP3_H

/// Structure.
typedef struct {
  void *e1;
  void *e2;
  void *e3;
} Tp3;

/// Constructor.
Tp3 *tp3_mk (void *e1, void *e2, void *e3);

#endif
