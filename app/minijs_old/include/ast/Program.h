// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array with program classes

#ifndef AST_PROGRAM_H
  # define AST_PROGRAM_H

#include <dmc/all.h>
#include "Class.h"
#include "Sym.h"

///
typedef struct program_Program Program;

///
Program *program_get(void);

///
size_t program_size(Program *this);

///
void program_add(Program *this, Class *class);

/// program_class returns the corresponding class or NULL
Class *program__class(Program *this, char *id);

/// program_att returns type of 'main' function.<p>
/// If fail throw a tx_exception.<p>
/// It tries to find 'id'. If it is not in program, read it from file system.<p>
/// When it reads, also read all dependencies and verifies correctness of
/// program.
/// Throws tx_exception.
Sym *program_att_main(char *class_id, Atype *generics);

/// program_att returns type of 'id'.<p>
/// If fail throw a tx_exception.<p>
/// It tries to find 'id'. If it is not in program, read it from file system.<p>
/// When it reads, also read all dependencies and verifies correctness of
/// program.
/// Throws tx_exception.
Type *program_att(Class *caller, char *class_id, Atype *generics, char *id);

#endif


