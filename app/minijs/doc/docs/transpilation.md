Transpilation
=============

Process
-------

#### Input data ####
`UserData *userData`

#### Ouput data ####
`Fails *fails`

#### Steps ####
1. Read main entry
    - **In**: `(Lchar *roots, Dep *mainFunction)`
    - **Out**: `JsEntryResult *result`
1. Test if errors
1. Test if is type is `()`
1. Print .js file
    - **In**: `(UserData *userData, JsEntry *entry)`
    - **Out**: `Void`

Types
-----

#### [Cpath](types/#cpath) ####
#### [Dep](types/#dep) ####
#### [Deps](types/#deps) ####
#### [Fail](types/#fail) ####
#### [Fails](types/#fails) ####
#### [JsEntry](types/#jsentry) ####
#### [JsEntryResult](types/#jsentryresult) ####
#### [Type](types/#type) ####


