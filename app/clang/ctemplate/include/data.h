// Copyright 24-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Structures to read templates.<br>
/// Definition in <a href="?*ctemplate@ctemplate">ctemplate</a>:

#ifndef DATA_H
  #define DATA_H

#include <stdbool.h>
typedef struct arr_Arr Arr;

/// Definitions
///   - : PRIVATE
///   + : STATIC (Makes annotation in .h)
///   default : PUBLIC (Makes annotation in .h)
enum func_Mod { FUNC_PRIVATE, FUNC_GET, FUNC_SET };

struct Func {
  enum func_Mod mod;
  char *id;
  char *type;
};

///
typedef struct Func Func;

/// Definitions
///   - : PRIVATE
///   + : SET (includes get)
///   default : GET
enum var_Mod { VAR_PRIVATE, VAR_GET, VAR_SET };

///
struct Var {
  enum var_Mod mod;
  bool func;
  char *id;
  char *type;
  char *value;
};

///
typedef struct Var Var;

/// Definitions
///   - : PRIVATE
///   + : SET (includes get)
///   default : GET
enum param_Mod { PARAM_PRIVATE, PARAM_GET, PARAM_SET };

///
struct Param {
  enum param_Mod mod;
  bool func;
  char *id;
  char *type;
  char *serial;
};

///
typedef struct Param Param;

///
struct Body {
  Arr/*Param*/ *ps;
  Arr/*Var*/ *vs;
  Arr/*Func*/ *fs;
};

///
typedef struct Body Body;

/// Definitions:
///   - : HEAD_NO_SERIAL
///   + : RESTORE (Includes serialization
///   default: SERIAL
enum head_Mod { HEAD_NO_SERIAL, HEAD_SERIAL };

///
enum head_Id_mod { HEAD_NEW_PUBLIC, HEAD_ID_PUBLIC, HEAD_ID_PRIVATE };

///
struct Head {
  enum head_Mod mod;
  enum head_Id_mod id_mod;
  char *id;
  char *prefix;
};

///
typedef struct Head Head;

///
struct Structure {
  Head *head;
  Body *body;
};

///
typedef struct Structure Structure;

///
struct Template {
  Arr/*Structure*/ *sts;
};

///
typedef struct Template Template;

#endif

