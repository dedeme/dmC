cnewfile
=========

Api
---

It is [here](http://localhost/dmcgi/CDoc/?*cnewfile)


Working
-------

Authomatic generation of empty 'c' and 'h' files.

Command line
------------

```text
cnewfile root relativePath
```
- **root**: It is the common parent of 'include' and 'src'
- **relativePath**: It is the path of '.h .c' files relative to 'root',
  without extension

For expample:
```text
cnewfile /home/frank/cprograms/oneprg test/save_test
```
will create two files:

```text
/home/frank/cprograms/oneprg/include/test/save_test.h
```
and
```text
/home/frank/cprograms/oneprg/src/test/save_test.c
```
