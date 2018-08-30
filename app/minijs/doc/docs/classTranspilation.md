Class transpilation
===================

Process
-------

#### Input data ####
`(Lchar *roots, Cpath *class)`

#### Output data ####
`JsClassResult *result`

#### Steps ####
1. Read skeleton
    - **In**: `(Lchar *roots, Scanner *text)`
    - **Out**: `SkeletonResult *result`
2. Read class:
    - **In**: `(Skeleton *skeleton, Scanner *text)`
    - **Out**: `JsClassResult *result`
3. Write class

Types
-----

#### [Cpath](types/#cpath) ####
#### [JsClass](types/#jsclass) ####
#### [JsClassResult](types/#jsclassresult) ####
#### [JsEntry](types/#jsentry) ####
#### [SkEntry](types/#skentry) ####
#### [Skeleton](types/#skeleton) ####
#### [SkeletonResult](types/#skeletonresult) ####
#### [Scanner](types/#scanner) ####
#### [Type](types/#type) ####
