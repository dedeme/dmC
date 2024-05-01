// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Low level code reader.

#ifndef CDR_H
  #define CDR_H

#include "kut/opt.h"
#include "reader/token.h"

typedef struct cdr_Cdr Cdr;

/// Constructor
///   fix: File index.
///   code: Code of corresponding file.
Cdr *cdr_new (int fix, char *code);

/// Returns the file index.
int cdr_get_file (Cdr *this);

/// Returns the current line number.
int cdr_get_nline (Cdr *this);

/// Returns the line number of the next token.
int cdr_get_next_nline (Cdr *this);

/// Returns an error message.
char *cdr_fail_line (Cdr *this, char *msg, int nline);

/// Returns an error message with the current line number.
char *cdr_fail (Cdr *this, char *msg);

/// Returns an expect-found error message.
char *cdr_fail_expect_line (Cdr *this, char *expect, char *found, int nline);

/// Returns an expect-found error message with the current line number.
char *cdr_fail_expect (Cdr *this, char *expect, char *found);

/// Returns '0' if end of code is reached.
char cdr_read_char (Cdr *this);

/// Back to the previous character. If the current character is in position 0
/// an EXC_ILLEGAL_STATE is thrown.
void cdr_unread_char (Cdr *this, char ch);

/// Reads a token in a Opt<Token>.
/// The return can be.
///   - opt_none -> End of text reached.
///   - opt_get -> Return the read token.
/// Throws Exception if an error heppened.
Opt *cdr_read_token_op (Cdr *this);

/// Reads a token.
/// Throws Exception if an error heppened or end of text is reached.
Token *cdr_read_token (Cdr *this);

/// Returns 'true' if the next token is the operator '.'.
int cdr_next_token_is_point (Cdr *this);

/// Returns 'true' if the next token is the operator ','.
int cdr_next_token_is_comma (Cdr *this);

/// Returns 'true' if the next token is the operator ':'.
int cdr_next_token_is_colon (Cdr *this);

/// Returns 'true' if the next token is the operator ';'.
int cdr_next_token_is_semicolon (Cdr *this);

/// Returns 'true' if the next token is the operator '!'.
int cdr_next_token_is_exclamation (Cdr *this);

/// Returns 'true' if the next token is the operator '!'.
int cdr_next_token_is_backslash (Cdr *this);

/// Returns 'true' if the next token is the operator '#'.
int cdr_next_token_is_hash (Cdr *this);

/// Returns 'true' if the next token is the operator '->'.
int cdr_next_token_is_arrow (Cdr *this);

/// Returns 'true' if the next token is a binary operator.
int cdr_next_token_is_binary (Cdr *this);

/// Returns 'true' if the next token is '?'.
int cdr_next_token_is_ternary (Cdr *this);

/// Returns 'true' if the next token is the operator '['.
int cdr_next_token_is_open_square (Cdr *this);

/// Returns 'true' if the next token is the operator ']'.
int cdr_next_token_is_close_square (Cdr *this);

/// Returns 'true' if the next token is the operator '('.
int cdr_next_token_is_open_parenthesis (Cdr *this);

/// Returns 'true' if the next token is the operator ')'.
int cdr_next_token_is_close_parenthesis (Cdr *this);

/// Returns 'true' if the next token is the operator '}'.
int cdr_next_token_is_close_bracket (Cdr *this);

/// Returns 'true' if the next token is the symbol 'else'.
int cdr_next_token_is_else (Cdr *this);

/// Returns 'true' if the next token is the symbol 'finally'.
int cdr_next_token_is_finally (Cdr *this);

#endif
