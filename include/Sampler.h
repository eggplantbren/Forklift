#ifndef Forklift_Sampler_h
#define Forklift_Sampler_h

#include <Database.h>
#include <Model.h>
#include <Stripe.h>
#include <tuple>

namespace Forklift
{

template<Model M>
class Sampler
{
    private:

        // Database for output
        Database database;

        // Random number generator
        Tools::RNG rng;

        // Particles tupled with their scalars
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
