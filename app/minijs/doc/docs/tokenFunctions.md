Syntactic Functions
=====================

All this functions have the commun skeme:

`(Opt *context, Scanner *scanner)` -> `ScanResult *scanResult`

- `context` Its value will be specified for each function.
- The field `result` of scanResult only is significant if there are not
  errors and its value also will be specified.


Blank
-----

char <= ' ' or comment.

- **In**: `null`
- **Out**: `null`
- **Error**: not blank or \0

#### Definition ####

`['\1-\32' | '/' '/' (~'\n')* '\n' | '/' '*' (~('*' '/'))* '*' '/']`


Char
------------

Read a character

- **In**: `char *`:
    - With one characer: The first character of str is used to test.
    - With two characters: Marks a range of characters, between the first
      and the second one, both inclusive.
- **out**: `char`
- **Error**: If the current char is not the first character of str or not is
    in range

#### Definition ####

`FINAL`


End
---

End of text reached.

- **In**: `null`
- **out**: `null`
- **Error**: Scanner is not at end of text.

#### Definition ####

`FINAL`


Id
--

Identifier

- **In**: `null`
- **out**: `char *`
- **Error**: First char is missing

#### Definition ####

`['a-z' | 'A-Z' | '$' | '_'] [ 'a-z' | 'A-Z' | '$' | '_' | '0-9' ]*`


Type
----

Read a type.

- **In**: `List<char*> *symbols`
- **Out**: `Type *`
- **Error**: Bad syntax or non existent symbols.

#### Definition ####

`[SingleType | FunctionType]`


Type/Function
-------------

Read a function type

- **In**: `List<char*> *symbols`
- **Out**: `Type *`
- **Error**: Bad syntax or non existent symbols.

#### Defintion ####

`'(' Blank* (Type (Blank* ',' Blank* Type)*)? Blank* ')'`


Type/Generics
-------------

Read a function type

- **In**: `List<char*> *symbols`
- **Out**: `List<Type *>`
- **Error**: Bad syntax or non existent symbols.

#### Defintion ####

`'<' Blank* (Type (Blank* ',' Blank* Type)*)? Blank* '>'`


Type/Single
-----------

Read a known identifier

- **In**: `List<char*> *symbols`
- **Out**: `Type *`
- **Error**: Non existent symbol.

#### Definition ####

`['*' | Id (Blank* GenericTypes)?]` // Id must be a known symbol.


