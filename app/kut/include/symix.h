// Copyright 11-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Object for mapping indexes to symbols.

#ifndef SYMIX_H
  #define SYMIX_H

/// Index of reserverd symbols.
enum symix_Reserved {
  // Reserverd words
  symix_BREAK, symix_CATCH, symix_CONTINUE, symix_DEFAULT,
  symix_ELSE, symix_FALSE, symix_FINALLY, symix_FOR,
  symix_IF, symix_IMPORT, symix_RETURN, symix_SWITCH,
  symix_TRACE, symix_TRUE, symix_TRY, symix_WHILE,

  symix_ENUM_SEPARATOR,
  // Built-in modules
  symix_ARR, symix_B64, symix_BYTES, symix_CRYP,
  symix_DIC, symix_ITER, symix_JS, symix_FILE,
  symix_MATH, symix_PATH, symix_REGEX, symix_STR,
  symix_SYS, symix_TCP, symix_THREAD, symix_TIME,
  symix_VEC,

  symix_ENUM_END
};

/// Initialize symbols array.
void symix_init(void);

/// If 'sym' is not in symbols array, it is added. Otherwise do nothing.
/// In any case this function returns ever the symbol index.
int symix_add(char *sym);

/// Returns the symnbol with index 'ix'.
/// If such index does no exist, it throw an exception.
char *symix_get (int ix);

/// Returns a new internal symbol.
int symix_generate (void);

#endif
