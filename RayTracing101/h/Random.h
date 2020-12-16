#ifndef __RANDOM_H_INCLUDED
#define __RANDOM_H_INCLUDED

//-- pseudorandom noise functions (random access / deterministic)
unsigned int Get1dNoiseUint(int idx, unsigned int seed = 0);
unsigned int Get2dNoiseUint(int idxX, int idxY, unsigned int seed = 0);
unsigned int Get3dNoiseUint(int idxX, int idxY, int idxZ, unsigned int seed = 0);

float Get1dNoise01(int idx, unsigned int seed = 0);
float Get2dNoise01(int idxX, int idxY, unsigned int seed = 0);
float Get3dNoise01(int idxX, int idxY, int idxZ, unsigned int seed = 0);

template <typename T>
T GetRandom01();

#endif