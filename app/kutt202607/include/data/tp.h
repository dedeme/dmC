// Copyright 13-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Tuple of 2 elements.

#ifndef DATA_TP_H
  #define DATA_TP_H

/// Structure.
typedef struct {
  void *e1;
  void *e2;
} Tp;

/// Constructor.
Tp *tp_mk (void *e1, void *e2);

#endif
