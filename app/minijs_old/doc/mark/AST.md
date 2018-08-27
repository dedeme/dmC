### [Top](index.html)
---------------------


AST
=============

Class ::= BL `_local`? BL [Generics](#generics)? BL [Import](#import)? BL [Extends](#extends)? BL [Implements](#implements)? BL [Public](#public)? BL [Private](#private)? BL END

<hr>
<a name="generics">
Generics ::= `<` BL [Id](#id),+ BL `>`
<hr>
<a name="import">
Import := `_import` BL [ImportDeclaration](#importDeclaration)+

<a name="importDeclaration">
ImportDeclaration ::= <br>
·· [FilePath](#filePath) BL [SymbolAssign](#symbolAssing)? |<br>
·· [DirPath](#dirPath) BL [ImportIncludes](#importIncludes) |<br>
·· [DirPath](#dirPath) BL [ImportMassive](importMassive)

<a name="filePath">
FilePath ::= `^/` ([Wpath](#wpath) `/`)* [Id](#id)

<a name="path">
DirPath ::= `.` | `^/` [Wpath](#wpath) (`/` [Wpath](#wpath))*

<a name="importIncludes">
ImportIncludes ::= `(` BL ([Id](#id) BL  [SymbolAssign](#symbolAssing)?),+ BL `)`

<a name="symbolAssign">
SymbolAssign ::= `<` BL [id](#id)

<a name="importMassive">
ImportMassive ::= `*` BL ( `(` BL [id](#id),+  BL `)` )?

<hr>
<a name="extends">
Extends ::= `_extends` BL ( [Tdata](#tdata) | [Tarray](#tarray) | [Tmap](#tmap) )

<hr>
<a name="implements">
Implements ::= `_implements` BL ( [Tdata](#tdata) | [Tarray](#tarray) | [Tmap](#tmap) )+

<hr>
<a name="public">
Public ::= `_public` BL [TopDeclaration](#topDeclaration)+

<a name="private">
Public ::= `_private` BL [TopDeclaration](#topDeclaration)+

<a name="topDeclaration">
TopDeclaration ::= [New](#new) | [Val](#val) | [Var](#var) | [Native](#native)

<hr>
<a name="new">
New ::= `new` BL [NewHead](#newHead) BL [NewBody](#newBody)

<a name="newHead">
NewHead ::= ( `+` | `-` )? BL [Id](#id),* BL `->`

<a name="newBody">
NewBody ::= [InstPublic]("#instPublic")? | [InstPrivate](#instPrivate)? | [Statement](#statement)*

<a name="instPublic">
InstPublic ::= `_public` BL [InstDeclaration](#instDeclaration)+

<a name="instPrivate">
InstPrivate ::= `_private` BL [InstDeclaration](#instDeclaration)+

<a name="instDeclaration">
InstDeclaration ::= [Val](#val) | [Var](#var) | [Native](#native)

<hr>
<a name="val">
Val ::= ( `:` BL [Type](#type) )? BL `val` BL [Id](#id) BL `=` BL [value](#Value)

<a name="var">
Var ::= ( `:` BL [Type](#type) )? BL `var` BL  [Id](#id) BL ( `;` |  ( `=` BL [value](#Value) ) )

<a name="native">
Native ::= `:` BL [Type](#type) BL `nat` BL  [Id](#id) BL `;`

<hr>
<a name="value">
Value ::= ( [Literal](#literal) | [Vid](#vid) | [Function](#function) | [Parenthesis](#parenthesis) | [Lunary]("lunary") | [Runary](#runary") | [Binary](#binary) | [Ternary](#ternary) )

<a name="literal">
Literal ::= ( Bool | Byte | Int | Float | Char | String | Array | Map )

<a name="vid">
Vid ::= [Id](#id) BL [Attach](#attach)*

<a name="attach">
Attach ::= [AttPoint](#attPoint) | [AttCall](#attCall) | [AttGet](#attGet)

<a name="attPoint">
AttPoint ::= `.` BL [Id](#id)

<a name="attCall">
AttCall ::= `(` BL [Value](#value),* BL `)`

<a name="attGet">
attGet ::= `[` BL [Value](#value) BL `]`

<a name="function">
Function ::= [FnHead](#fnHead) BL [Block](#block)

<a name="fnHead">
FnHead ::= [Id](#id),* BL `->`

<a name="parenthesis">
Parenthesis ::= `(` BL [Value](#value) BL `)`

<a name="lunary">
Lunary ::= ( `+` | `-` | `!` | `~` ) BL [Value](#value)  | ( `++` | `--` ) BL [Id](#id)

<a name="runary">
Runary ::= [Id](#id) BL ( `++` | `--` )

<a name="binary">
Binary ::= [Value](#value) BL [BinOp](#binOp) BL [Value](#value)

<a name="binOp">
BinOp ::= `+` | `-` ... *etc*

<a name="ternary">
Ternary ::= [Value](#value) BL `?` BL [Value](#value) BL `:` BL [Value](#value)

<hr>
<a name="statement">
Statement ::=


<a name="block">
Block ::= [Statement](#statement) | `{` BL [Statement](#statement)* BL `}`

<hr>
<a name="type">
Type ::= [Tdata](#tdata) | [Tfunction](#tfunction) | [Tany](#tany) | [Tarray](#tarray) | [Tmap](#tmap)

<a name="tdata">
Tdata ::= [Id](#id) BL (`<` BL [Type](#type),+ BL `>` )?

<a name="tfunction">
Tfunction ::= `(` BL [Type](#type),* BL `:` BL [Type](#type)? BL  `)`

<a name="tany">
Tany ::= `*`

<a name="tarray">
Tarray ::= `[` BL [Type](#type) BL `]`

<a name="tmap">
Tmap ::= `{` BL [Type](#type) BL `}`

<a name="id">
Id ::= [$a-zA-z] [$a-zA-z0-9]

<a name="wpath">
Wpath ::= (*character != '/' && above ' '*)+

<hr>

BL ::= (*character with value above <> \0*)*

END ::= `\0`
