// Copyright 05-Jun-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Company table.

#ifndef DB_COS_TB_H
  #define DB_COS_TB_H

#include "data/cosT.h"

/// Initialize table.
void cosTb_init (void);

/// Returns companies in quotes data base.
CosT *cosTb_read (void);

/// Select - Deselect a company.
///   nick: Company nick.
///   value: If it is 'TRUE' 'nick' is selected. Oterwise it is deselected.
void cosTb_select (char *nick, int value);

/// Make 'nick' the main company.
void cosTb_setMain (char *nick);

#endif
