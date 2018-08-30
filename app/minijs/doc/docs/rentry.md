Entry reading
=============

Process
-------

#### Input data ####
`(Lchar *roots, Dep *mainFunction)`

#### Ouput data ####
`JsEntryResult *result`

#### Steps ####
1. `jsClassResult` =
    - It is in cache -> it reads from and make a valid `JsClassResult`
    - It is not: Write the complete class in cache and returns it
        - **In**: `(Lchar *roots, Cpath *class)`
        - **Out**: `JsClassResult *result`
1. Returns the `JsEntryResult` from `jsClassResult`.

Types
-----

#### [Cpath](types/#cpath) ####
#### [Dep](types/#dep) ####
#### [Deps](types/#deps) ####
#### [JsClass](types/#jsclass) ####
#### [JsClassResult](types/#jsclassresult) ####
#### [JsEntry](types/#jsentry) ####
#### [JsEntryResult](types/#jsentryresult) ####
#### [Type](types/#type) ####
