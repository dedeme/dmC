### [Top](index.html)
---------------------

File Process
============

Installation
---------------

It must exist `/home/.dmCApp/minijs/paths` and `/home/.dmCApp/minijs/built`

User data
---------

User must give:
* Source main file name.
* Target file.
* Directory paths for searching source files.

Main Process
------------

let's do
> `main` = Main files <br>
> `index` = Target file <br>
> `paths` = search paths <br>
> `Class` = Initial class <br>
> `Fclass` = Final class

1. Add paths '.' and 'built' to `paths`.
1. Initialize global objects `Cpath`, ...
1. Read initial class from `main` and store it in an array (`Aclass`). See [class reading](classReading.html).
1. Parse main final class from `Class` and store it in an array (`Afclass`)
1. Return the type of function main and tests its type (= `(:)`) and its attributes (`public = true` and `val = true`)
1. Write `index`.
