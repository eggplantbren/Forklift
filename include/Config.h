#ifndef Forklift_Config_h
#define Forklift_Config_h

#include <tuple>

namespace Forklift
{

// A singleton
struct Config
{
    static int num_particles;
    static int mcmc_steps;
    static std::pair<int, int> depth_nats;
    static double thin;
    static int rng_seed;
    static int num_threads;
    static void load(const char* filename);
};

} // namespace

#endif
