#ifndef Forklift_Stripe_h
#define Forklift_Stripe_h

#include "Model.h"
#include <Tools/RNG.hpp>

namespace Forklift
{


template<Model M>
using Particle = std::tuple<M, std::vector<double>>;


template<Model M>
class Stripe
{
    private:

        // Stripe number
        int stripe_id;

        // Particles
        std::vector<M> particles;

        // Threshold
        double xstar, ystar;

    public:

        // No default constructor
        Stripe() = delete;

        // Constructor
        Stripe(int _stripe_id,
               const std::vector<Particle<M>>& _particles,
               double _xstar);

        // Do one NS iteration
        void ns_iteration(Tools::RNG& rng);
};

} // namespace

#endif

#include "StripeImpl.h"
