Entry reading
=============

Process
-------

#### Input data ####
`(UserData *userData, Errors *errors, Dep *mainFunction)`

#### Ouput data ####
`JsEntryResult *result`

#### Steps ####
1. `jsClassResult` =
    - It is in cache -> it reads from and make a valid `JsClassResult`
    - It is not: Write the complete class in cache and returns it
        - **In**: `(UserData *userData, Errors *errors, Cpath *class)`
        - **Out**: `JsClassResult *result`
1. Returns the `JsEntryResult` from `jsClassResult`.

Types
-----

#### UserData ####
```c
struct {
  Lchar *roots; // Directories to search Classes
  char *source; // Main file .minijs
  char *target; // Target file .js
}

```
#### Errors ####
```c
struct {
  Lerror *errors;   // Errors list (until ten and in reverse order)
  Lerror *warnings; // Warnings list (until ten and in reverse order)
}
```

#### Error [L_Error] ####
```c
struct {
  char *file; // Path relative to root
  int nline;  // 1 based line number
  int nchar;  // 0 based number char
}
```
#### JsEntryResult ####
```c
struct {
  Errors *errors; // Errors and warning lists
  JsEntry *entry; // Js Class entry
}
```

#### JsEntry ####
```c
struct {
  Type *type;         // Entry type
  char *js;           // js text transpiled
  Deps *dependencies; // Identifiers of JsEntry dependencies
}
```

#### Type ####
```c
  bool is_function; // It type is a function
  char *id;         // Type identifier
  Atype *params;    // Arguments in functions and generics otherwise.
}
```

#### Dep ####
```c
struct {
  Cpath *class;   // Class path
  char *entry;     // Identifier of entry
}
```

#### Deps ####
```c
struct {
  Arr<Dep> *deps // Opaque
}
```

#### Deps ####
```c
struct {
  Arr<Dep> *deps // Opaque
}
```

#### Cpath ####
```c
struct {
  char *root;     // Parent directory.
  char *relative; // Path relative to root.
}
```

#### JsClassResult ####
```c
struct {
  Errors *errors; // Errors and warning lists
  JsClass *class; // Js class
}
```

#### JsClass ####
```c
struct {
  MjsEntry *entries; // Map with entries
}
```
