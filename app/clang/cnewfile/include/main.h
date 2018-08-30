// Copyright 30-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Authomatic generation of empty 'c' and 'h' files.
///   Use: cnewfile root relativePath
///   root: It is the common parent of 'include' and 'src'
///   relativePath: It is the path of '.h .c' files relative to 'root',
///     without extension
///   For expample:
///   cnewfile /home/frank/cprograms/oneprg test/save_test
///   will create two files:
///   /home/frank/cprograms/oneprg/include/test/save_test.h
///   and
///   /home/frank/cprograms/oneprg/src/test/save_test.c

#ifndef MAIN_H
  #define MAIN_H

///
int main (int argc, char **argv);

#endif
