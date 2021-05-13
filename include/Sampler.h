#ifndef Forklift_Sampler_h
#define Forklift_Sampler_h

#include <Database.h>
#include <Double.h>
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

        // Particles with their scalars
        std::vector<M> particles;
        std::vector<Double> xs, ys;

        // Iteration counter
        int stripe_id, iteration;

        // Threshold for first scalar
        Double xstar;

        // Do one NS iteration wrt x
        void ns_iteration();

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
