#ifndef Forklift_Sampler_h
#define Forklift_Sampler_h

#include <ctime>
#include <Config.h>
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
        std::vector<Tools::RNG> rngs;

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

        Sampler();

        // Private functions
        void update();
};

} // namespace

#endif

#include "SamplerImpl.h"
