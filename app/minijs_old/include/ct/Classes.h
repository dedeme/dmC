// Copyright 04-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Container for Classes

#ifndef CT_CLASSES_H
  #define CT_CLASSES_H

typedef struct cpath_Cpath Cpath;
typedef struct class_Class Class;
typedef struct oclass_Oclass Oclass;

///
void classes_init(void);

///
bool classes_contains(Cpath *path);

///
void classes_add(Class *c);

///
Oclass *classes_get(Cpath *path);

#endif
