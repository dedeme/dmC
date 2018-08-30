Types
=====

Cpath
-----
```c
struct {
  char *root;     // Parent directory.
  char *relative; // Path relative to root.
}
```

Dep
---
```c
struct {
  Cpath *class;   // Class path
  char *entry;     // Identifier of entry
}
```

Deps
----
```c
struct {
  Arr<Dep> *deps // Opaque
}
```

Fail
-----
```c
struct {
  char *file; // Path relative to root
  int nline;  // 1 based line number
  int nchar;  // 0 based number char
}
```

Fails
------
```c
struct {
  Afail *fails;   // Errors list (until ten and in reverse order)
  Afail *warnings; // Warnings list (until ten and in reverse order)
}
```

JsClass
-------
```c
struct {
  MjsEntry *entries; // Map with entries
}
```

JsClassResult
-------------
```c
struct {
  Fails *fails; // Errors and warning lists
  JsClass *class; // Js class
}
```

JsEntry
-------
```c
struct {
  Type *type;         // Entry type
  char *js;           // js text transpiled
  Deps *dependencies; // Identifiers of JsEntry dependencies
}
```

JsEntryResult
-------------
```c
struct {
  Fails *fails; // Errors and warning lists
  JsEntry *entry; // Js Class entry
}
```

Scanner
-------
```c
struct {
  Opaque. // Device to scan text and make error messages.
}
```

ScanResult
----------
```c
sctruct {
  Fails *fails; // Fails of scanning
  void *result; // Object read if not error
  Scanner *scanner // Final status of scanner
}
```

SkEntry
-------
```c
struct {
  Type *type; // Entry type
}
```

Skeleton
--------
```c
struct {
  MjsSkEntry *entries; // Map with skeleton entries
}
```

SkeletonContext
---------------
```c
struct {

  Achar *typeNames; // Names of knwon types.
}
```

SkeletonResult
--------------
```c
struct {
  Fails *fails; // Errors and warning lists
  Skeleton *skeleton; // Js class
}
```

UserData
--------

```c
struct {
  Lchar *roots; // Directories to search Classes
  char *source; // Main file .minijs
  char *target; // Target file .js
}
```

Type
----
```c
  bool is_function; // It type is a function
  char *id;         // Type identifier
  Atype *params;    // Arguments in functions and generics otherwise.
}
```
