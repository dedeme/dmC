// Copyright 09-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef TYPES_DTYPE_H
  # define TYPES_DTYPE_H

#include <dmc/all.h>
#include "DEFS.h"

/*. - .*/

///
typedef struct dtype_Dtype Dtype;

///
enum Type_t dtype_type(Dtype *this);

///
char *dtype_id(Dtype *this);

///
Arr/*Dtype*/ *dtype_generics(Dtype *this);

///
Arr/*Dtype*/ *dtype_params(Dtype *this);

/// It can return a null value.
Dtype *dtype_super(Dtype *this);

///
Arr/*Dtype*/ *dtype_implements(Dtype *this);

///
Map/*Dtype*/ *dtype_objects(Dtype *this);

/*. - .*/

Dtype *dtype_new_data(
  char *id,
  Arr *generics,
  Arr *params,
  Dtype *super,
  Arr *implements
);


#endif
