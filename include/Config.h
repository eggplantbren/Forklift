#ifndef Forklift_Config_h
#define Forklift_Config_h

namespace Forklift
{

// A singleton
struct Config
{
    static int num_particles;
    static int mcmc_steps;
    static int depth_nats;
    static double thin;
    static void load(const char* filename);
};


} // namespace

#endif
