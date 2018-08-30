ctemplate
=========

Api
---

It is [here](http://localhost/dmcgi/CDoc/?*ctemplate)


Working
-------

It reads a set of .c files, usin a comment which starts with "/*.+." to
generate code for header (.h) and body (.c) files.

This generated code is written between coments "/*.-.*/".

Examples
--------

Code .c:

```c
// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/*.
struct: Cpath
  root: char *: _string
  relative: char *: _string
*/

/*.-.*/
/*.-.*/
```

Code .h:

```c
// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef TYPES_CPATH_H
  # define TYPES_CPATH_H

/*.-.*/
/*.-.*/

#endif
```

Command line
------------

_ctemplate_ needs an argument:

```text
ctemplate path
```

`path` is the root of two directories "include" and "src".

_ctemplate_ look up .c files with template in "src" and all their subdirectories.
When it finds one, modifies it and its corresponding .h file. This .h file must
be placed in the same relative directory below "include".


Syntax
------

TEMPLATE:
```text
/*[.+.]
STRUCTRURE+
*/
```

STRUCTURE:
```text
HEAD
  [BODY]
```

HEAD:
```text
[-]struct: [-|@]Id
```
- **[-]**:
    - ** - **: The structure will not be serialized.
    - ** Default**: The structure will be serialized.
- **[-|@]**:
    - ** - **: Neither structure nor 'new' will be public
    - ** @ **: Structure will be public, but 'new' not
    - ** Default **: Structure and 'new' will be public

BODY:
```text
  [PARAM+]
  [-
  VAR+]
  [-
  [FUNC+]
```
- **PARAM**: Argument of 'new'.
- **VAR**: Variable out of 'new'.
- **FUNC**: Function inside structure.


PARAM:
```text
  [+|-]id: type [: fserial]
```
- **[+|-]**:
    - ** + **: With getter and setter
    - ** - **: Without getter or setter
    - ** Default **: With getter but without setter
- **: fserial**: (Only if structure is going to be serialized)
    - ** _bool **: Serialize with jarr_abool() and restore with jarr_gbool()
    - ** _int **: Serialize with jarr_aint() and restore with jarr_gint()
    - ** _uint **: Serialize with jarr_auint() and restore with jarr_guint()
    - ** _string **: Serialize with jarr_astring() and restore with
        jarr_gstring()
    - ** _sarray **: Serialize wint jarr_aarray() using 'json_wstring()' as
        function 'to()' and 'json_rstring()' as function 'from()'
    - ** _sobject **: Serialize wint jarr_aobject() using 'json_wstring()' as
        function 'to()' and 'json_rstring()' as function 'from()'
    - ** _double n **: Serialize with jarr_adouble() using 'n' decimals number
        and restore with jarr_gdouble()
    - ** fn **: Serialize with 'fn_to_json()' and restore with
        'fn_from_json()'
    - ** _array fn **: Serialize with jarr_aarray() using 'fn_to_json()' as
        function 'to()' and 'fn_form_json()' as function 'from()'
    - ** _object fn **: Serialize with jarr_aobject() using 'fn_to_json' as
        function 'to()' and 'fn_from_json()' as function 'from()'

VAR:
```text
  [+|-]id: type = value
```
- **[+|-]**:
    - ** + **: With getter and setter
    - ** - **: Without getter or setter
    - ** Default **: With getter but without setter

FUNC:
```text
  [+|-]id: type
```
- **[+|-]**:
    - ** + **: With getter and setter
    - ** - **: Without getter or setter
    - ** Default **: With getter but without setter
