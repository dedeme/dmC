// Copyright 19-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Symbol Class attribute.<p>

#ifndef AST_CSYM_H
  # define AST_CSYM_H

typedef struct type_Type Type;

/*.-.*/

///
typedef struct csym_Csym Csym;

///
Csym *csym_new(
  bool is_public,
  bool is_static,
  char *class_id,
  char *id,
  Type *type
);

///
bool csym_is_public(Csym *this);

///
bool csym_is_static(Csym *this);

///
char *csym_class_id(Csym *this);

///
char *csym_id(Csym *this);

///
Type *csym_type(Csym *this);

/*.-.*/

#endif
