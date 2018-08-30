Logic Functions
===============

This functions use special types. See [here](astTypes).

And
---

Call f(context, scanner) until fail

- **In**: `AstArr`
- **Out**: `Arr<void *>` // Heterogeneous array
- **Error**: Any application of 'f' fails.


None_or_several (*)
-------------------
Call f(context, scanner) until fail

- **In**: `AstTp`
- **Out**: `List<void *>`
- **Error**: Never fails.


Not (~)
-------

Fail if f(context, scanner) successes.

- **In**: `AstMsg` // With one (one char) or two (group) characters.
- **Out**: `opt_none()`
- **Error**: The character is \0, is the supplied or is in the group.


One_or_more (+)
---------------

Call f(context, scanner) until fail

- **In**: `AstTp`
- **Out**: `List<void *>`
- **Error**: First application of 'f' fails.


One_or_none (?)
---------------

Call f(context, scanner) once

- **In**: `AstTp`
- **Out**: `Opt *`
- **Error**: Never fails.


Or
--

Call f(context, scanner) until success

- **In**: `AstArr`
- **Out**: `void *`
- **Error**: All the applications of 'f' fail.

