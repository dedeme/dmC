// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities to manage exceptions.<p>
/// Only functions 'exc_init', 'exc_stack'  and 'exc_throw' must be used
/// directly. The rest must be used through the macros TRY-CATCH-_TRY and
/// THROW-_THROW. (see <a href="?libdm@DEFS#hp:TRY">DEFS</a>)

#ifndef DM_EXC_H
  #define DM_EXC_H

#include <setjmp.h>

typedef struct achar_Achar Achar;

/// exc_init intializes jumps buffer. This function has to be called before
/// using macros TRY-CATCH-FINALLY-_TRY or THROW-_THROW.
void exc_init();

/// Adds a exception to buffer
void exc_add(jmp_buf *jump);

/// Remove the top of jumps buffer.
void exc_remove();

/// exc_type returns the last exception type.
char *exc_type();

/// exc_msg returns the last exception message.
char *exc_msg();

/// exc_stack returns the last exception stack.
Achar *exc_stack();

/// exc_throw sends an exception.<p>
/// If no TRY block has been defined it stops the program.///
///   type   : Excepion type
///   message: Message to show
///   file   : Error file
///   func   : Error function
///   line   : Error line number
void exc_throw(
  char *type, char *message, char *file, char *func, int line
);

///
#define exc_range_t "range"

/// exc_range. Exception for index out of range.
///   begin: Lower limit inclusive
///   end  : Upper limit exclusive
///   index: The index out of range
char *exc_range(int begin, int end, int index);

///
#define exc_null_pointer_t "null"

/// exc_null_pointer. Exception for argument with a NULL value.
///   argument_name: Name of wrong argument
char *exc_null_pointer(char *argument_name);

///
#define exc_illegal_argument_t "argument"

/// exception_illegal_argument. Exception for argument with a wrong value.
///   argument_name: Name of wrong argument
///   expected: Value expected
///   actual: Actual value
char *exc_illegal_argument(char *argument_name, char *expected, char *actual);

///
#define exc_illegal_state_t "state"

/// Attempt to use an object in bad condition.
///   cause: Description of problem
char *exc_illegal_state(char *cause);

///
#define exc_io_t "io"

/// exception_io. Exception for Input - Output error.
///   cause: Description of problem
char *exc_io(char *cause);

#endif
