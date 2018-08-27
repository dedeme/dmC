
Global process
==============
---

1. Read arguments
    - **In**: `(int argc, char ** args)`
    - **Out**: `Opt<UserData> *userData`
- Make transpilation
    - **In**: `UserData *userData`
    - **Out**: `Errors *errors`
- Cleaning and messages
    - **In**: `(char *target, `Errors *errors)`
    - **out**: ()

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
