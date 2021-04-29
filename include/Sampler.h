#ifndef Forklift_Sampler_h
#define Forklift_Sampler_h

#include <Database.h>
#include <Model.h>

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

        // Particles and their scalars
        std::vector<M> particles;
        std::vector<std::vector<double>> scalars;

        // Stripe limits. If empty, we're producing the floor.
        std::vector<double> stripe;

        // Threshold
        std::vector<double> threshold;

        // Private functions
        void update();

    public:

        // Construct with a random number generator
        Sampler(Tools::RNG&& _rng);
        Sampler() = delete;

        void run_to_depth(int nats);
};

} // namespace

#endif

#include "SamplerImpl.h"
