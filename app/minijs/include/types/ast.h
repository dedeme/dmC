// Copyright 28-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Ast types
#ifndef TYPES_AST_H
  # define TYPES_AST_H

#include "ScanResult.h"
#include "dmc/It.h"

typedef ScanResult *(*AstFn) (void *context, Scanner *scanner);

/// Ast function:
///   typedef ScanResult *(*AstFn) (void *context, Scanner *scanner);
typedef AstFn AstFn;

/// Record {void *context, AstFn fn}
typedef struct astTp_AstTp AstTp;

/// Record {void *context, AstFn fn, char *msg}
typedef struct astMsg_AstMsg AstMsg;

/// Creates a new AstTp
AstTp *ast_tp_new(void *context, AstFn fn);

/// Returns the context of 'this'
void *ast_tp_context(AstTp *this);

/// Returns the function of 'this'
AstFn ast_tp_fn(AstTp *this);

/// Creates a new AstMsg
AstMsg *ast_msg_new(void *context, AstFn fn, char *msg);

/// Returns the message of 'this'
char *ast_msg_msg(AstMsg *this);

/// Cast to AstTp which allows call to 'ast_tp_context' and 'ast_tp_fn'.
AstTp *ast_msg_cast(AstMsg *this);

/// Array of AstTp
typedef struct astArr_AstArr AstArr;

/// Creates a new AstArr
AstArr *ast_arr_new();

/// Add elements to 'this'
void ast_arr_add(AstArr *this, AstTp *tp);

/// Returns an iterator over 'this'.
It/*AstTp*/ *ast_arr_cast(AstArr *this);

#endif
