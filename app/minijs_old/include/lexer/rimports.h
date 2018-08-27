// Copyright 06-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Reads Class imports
///   Import ::= '_import' BL (<i>ImportDeclaration</i>)+
///   ImportDeclaration ::=
///     <i>FilePath</i> BL <i>SymbolAssign</i>? |
///     <i>DirPath</i> BL <i>ImportIncludes</i> |
///     <i>DirPath</i> BL <i>ImportMassive</i>
///   FilePath ::= ^'/' (<i>Wpath</i> '/')* <i>Id</i>
///   DirPath ::= '.' | ^'/' <i>Wpath</i> ('/' <i>Wpath</i>)*
///   ImportIncludes ::= '(' BL (<i>Id</i> BL <i>SymbolAssign</i>)+, BL ')'
///   SymbolAssign ::= '<' BL <i>Id</i>
///   ImportMassive ::= '*' BL ('(' BL <i>Id</i>+, BL ')')?
///   Id ::= [$a-zA-z] [$a-zA-z0-9]
///   Wpath >>= (Character != '/' & > ' ')+
///   BL >>= (blanks & comments)+

#ifndef LEXER_RIMPORTS_H
  #define LEXER_RIMPORTS_H

typedef struct asym_Asym Asym;
typedef struct hchar_Hchar Hchar;
typedef struct tx_Tx Tx;

///
Tx *rimports(Hchar **imports, Tx *tx, Asym *symbols, Cpath *path);

#endif
