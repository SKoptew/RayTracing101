#include "h/Random101.h"
#include <random>

static std::mt19937 gen{ std::random_device() () };

real Rand01()
{   
    //static std::uniform_real_distribution<real> distr{ 0, 1 };
    //return distr(gen);

    return std::generate_canonical<real, std::numeric_limits<real>::digits>(gen);
}

