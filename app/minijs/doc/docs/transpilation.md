Transpilation
=============

Process
-------

#### Input data ####
`UserData *userData`

#### Ouput data ####
`Errors *errors`

#### Steps ####
1. Read main entry
    - **In**: `(UserData *userData, Errors *errors, Dep *mainFunction)`
    - **Out**: `JsEntryResult *result`
1. Test if errors
1. Test if is type is `()`
1. Print .js file
    - **In**: `(UserData *userData, JsEntry *entry)`
    - **Out**: `Void`

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

#### Deps ####
```c
struct {
  Arr<Dep> *deps // Opaque
}
```

#### Dep ####
```c
struct {
  Cpath *class;   // Class path
  char *entry;     // Identifier of entry
}
```

#### Cpath ####
```c
struct {
  char *root;     // Parent directory.
  char *relative; // Path relative to root.
}
```


