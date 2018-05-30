// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// List structure
#ifndef DM_LIST_H
  #define DM_LIST_H

///
typedef struct list_List List;

///
List *list_new(void);

/// list_head returns every element of 'this' less the first one. If "this" is
/// not empty, throws an exception
List *list_tail(List *this);

/// list_head returns the first element of 'this'. If "this" is not empty,
/// throws an exception.
void *list_head (List *this);

/// list_cons adds 'o' at head.
List *list_cons(List *this, void *o);

#endif
