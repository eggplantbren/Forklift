#ifndef Forklift_Sampler_h
#define Forklift_Sampler_h

#include <Database.h>
#include <Model.h>
#include <Stripe.h>

namespace Forklift
{

/*
* A Sampler is a single run of Nested Sampling,
* either to produce the floor or a stripe.
*/

template<Model M>
class Sampler
{
    private:

        // Database for output
        Database database;

        // Random number generator
        Tools::RNG rng;

        // Particles
        std::vector<M> particles;

        // Iteration counter
        int iteration;

        // Threshold for first scalar
        double xstar;

    public:

        // Construct with a random number generator
        Sampler(Tools::RNG&& _rng);
        Sampler() = delete;

        // Private functions
        void update();
};

} // namespace

#endif

#include "SamplerImpl.h"
