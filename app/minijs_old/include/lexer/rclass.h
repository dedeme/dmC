// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Reads a Class in Classes
///   Class ::= BL _local?
///     BL <i>Generics</i>?
///     BL <i>Import</i>?
///     BL <i>Extends</i>?
///     BL <i>Implements</i>?
///     BL <i>Public</i>?
///     BL <i>Private</i>?
///     BL END
///   Generics ::= < BL <i>Id</i>,+ BL >
///   Imports ::= <a href='?*minijs@lexer/rimports'>rimports</a>
///   Extends ::= '_extends' BL <i>Tdata</i> | <i>Tarray</i> | <i>Tmap</i>
///   Implements ::= '_implements' BL
///     (<i>Tdata</i> | <i>Tarray</i> | <i>Tmap</i>)+
///   Public := <a href='?*minijs@lexer/rcatts'>rcatts</a>
///   Private := <a href='?*minijs@lexer/rcatts'>rcatts</a>
///   Tdata := <i>Id</i> BL ('<' BL <i>Type</i>,+ BL '>' )?
///   Tarray ::= '[' BL <i>Type</i> BL ']'
///   Tmap ::= '{' BL <i>Type</i> BL '}'
///   Type := <a href='?*minijs@lexer/rtype'>rtype</a>
///   Id ::= [$a-zA-z] [$a-zA-z0-9]
///   BL >>= (blanks & comments)+
///   END >>= End of text

#ifndef LEXER_RCLASS_H
  #define LEXER_RCLASS_H

typedef struct cpath_Cpath Cpath;

///
void rclass(Cpath *path);

#endif
