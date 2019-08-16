// Copyright 24-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef TPL_H
  #define TPL_H

#include "dmc/std.h"
#include "DEFS.h"
#include "Log.h"

/// Argument parameters
typedef struct tpl_TplArgument TplArgument;

///
TplArgument *tplArgument_new (void);

/// Opt[char]
Opt *tplArgument_comment (TplArgument *this);

///
void tplArgument_set_comment (TplArgument *this, char *doc);

///
enum AttType tplArgument_access (TplArgument *this);

///
void tplArgument_set_access (TplArgument *this, enum AttType type);

///
char *tplArgument_name (TplArgument *this);

///
char *tplArgument_type (TplArgument *this);

/// Retuns an error if name is empty or type is not valid. Otherwise returns ""
char *tplArgument_set_name_type (TplArgument *this, char *name, char *type);


// ------------------- //
// ------------------- //

/// Variable and function parameters
typedef struct tpl_TplVariable TplVariable;

///
TplVariable *tplVariable_new (void);

/// Opt[char]
Opt *tplVariable_comment (TplVariable *this);

///
void tplVariable_set_comment (TplVariable *this, char *doc);

///
enum AttType tplVariable_access (TplVariable *this);

///
void tplVariable_set_access (TplVariable *this, enum AttType type);

///
char *tplVariable_name (TplVariable *this);

///
char *tplVariable_type (TplVariable *this);

///
char *tplVariable_value (TplVariable *this);

/// Retuns an error if name is empty or type is not valid. Otherwise returns ""
char *tplVariable_set_name_type_value (
  TplVariable *this, char *name, char *type, char *value
);

/// Retuns an error if name is empty or type is not valid. Otherwise returns ""
char *tplVariable_set_function (TplVariable *this, char *def, char *value);

/// Cast to TplArgument
TplArgument *tplVariable_to_tplArgument (TplVariable *this);

// ------------------- //
// ------------------- //

///
typedef struct tpl_Tpl Tpl;

///
Tpl *tpl_new (void);

/// Returns Opt[char]
Opt *tpl_struct_comment (Tpl *this);

///
void tpl_set_struct_comment (Tpl *this, char *doc);

///
enum StructType tpl_struct_type (Tpl *this);

///
void tpl_set_struct_type (Tpl *this, enum StructType type);

///
enum ConstructorType tpl_constructor_type (Tpl *this);

///
void tpl_set_constructor_type (Tpl *this, enum ConstructorType type);

///
enum SerialType tpl_serial_type (Tpl *this);

///
void tpl_set_serial_type (Tpl *this, enum SerialType type);

///
char *tpl_struct_name (Tpl *this);

///
void tpl_set_struct_name (Tpl *this, char *name);

/// Returns Arr[TplArgument]
Arr *tpl_arguments (Tpl *this);

///
int tpl_arguments_size (Tpl *this);

///
void tpl_add_argument (Tpl *this, TplArgument *ar);

/// Returns Arr[TplVariable]
Arr *tpl_variables (Tpl *this);

///
void tpl_add_variable (Tpl *this, TplVariable *var);

/// Returns Arr[TplVariable]
Arr *tpl_functions (Tpl *this);

///
void tpl_add_function (Tpl *this, TplVariable *fun);

///
char *tpl_to_str (Tpl *this);

#endif
