Skeleton syntax functions
=========================

All this functions have the commun skeme:

`(Opt *context, Scanner *scanner)` -> `ScanResult *scanResult`

- `context` Its value will be specified for each function.
- The field `result` of scanResult only is significant if there are not
  errors and its value also will be specified.

`SkTopDeclaration` use as context the type `SkeletonContext`.

SkTopDeclaration
----------------

Read a top declaration.

- **In**: `Lchar *typeNames`
- **Out**: `Tp<Type *, char *> typeId`
- **Error**: logic_and

#### Definition ####

`'v' 'a' 'r'? Blank+ Type Blank* ':' Blank* Id Blank* [';' | '=' SkValue]`


SkValue
-------

Read schematically a value.

- **In**: `null`
- **Out**: `null`
- **Error**: logic_or

#### Definition ####

`[SkSingleValue | SkFunction]`


SkSingleValue
-------------

Read code until ';'. End of text is an error.

Scanner rests after ';'

- **In**: `null`
- **Out**: `null`
- **Error**: End of text found.

#### Definition ####

`(Blank* ~';')* ';'`


SkFunction
----------

Read code until '{' and then read code until '}'. End of text is an error.

Inside '{' '}' other brakets will be skipped.

Scanner rests after '}'

- **In**: `null`
- **Out**: `null`
- **Error**: End of text found.

#### Definition ####

`'{' [ Blank+ | SkFunction | ~'}' ]* '}'`


Logic Functions
---------------

#### [And](logicFunctions/#and) ####
#### [None or several (*)](logicFunctions/#none_or_several) ####
#### [Not](logicFunctions/#not) ####
#### [Not 0](logicFunctions/#not0) ####
#### [One or more (+)](logicFunctions/#one_or_more) ####
#### [One or none (?)](logicFunctions/#one_or_none) ####
#### [Or](logicFunctions/#or) ####


Syntax Functions
----------------

#### [Blank](tokenFunctions/#blank) ####
#### [Id](tokenFunctions/#id) ####
#### [Type](tokenFunctions/#type) ####


Types
-----

#### [Scanner](types/#scanner) ####
#### [ScanResult](types/#scanresult) ####
#### [SkeletonContext](types#skeletoncontext ####
#### [Type](types/#type) ####
