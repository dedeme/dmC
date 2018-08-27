// Copyright 04-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Container for Fclasses

#ifndef CT_FCLASSES_H
  #define CT_FCLASSES_H

typedef struct cpath_Cpath Cpath;
typedef struct fclass_Fclass Fclass;
typedef struct ofclass_Ofclass Ofclass;

///
void fclasses_init(void);

///
bool fclasses_contains(Cpath *path);

///
void fclasses_add(Fclass *c);

///
Ofclass *fclasses_get(Cpath *path);

#endif
