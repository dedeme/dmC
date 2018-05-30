// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Generator of random numbers

#ifndef DM_RND_H
  #define DM_RND_H

/// rnd_init intializates the random number generator.
void rnd_init (void);

/// rnd_d generates a new double between 0.0 (inclusive) and 1.0 (exclusive)
double rnd_d (void);

/// rnd_i generates a new int between 0 (inclusive) and 'top' (exclusive)
size_t rnd_i (size_t top);

#endif
