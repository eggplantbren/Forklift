#ifndef Forklift_Stripe_h
#define Forklift_Stripe_h

#include "Database.h"
#include "Model.h"
#include <Tools/RNG.hpp>
#include <vector>

namespace Forklift
{

template<Model M>
class Stripe
{
    private:

        // Stripe number
        int stripe_id;

        // Particles and their y-values
        std::vector<M> particles;
        std::vector<double> ys;

        // Threshold
        double xstar, ystar;

        // Iteration
        int iteration;

    public:

        // No default constructor
        Stripe() = delete;

        // Constructor
        Stripe(int _stripe_id,
               const std::vector<M>& _particles,
               double _xstar);

        // Do one NS iteration
        void ns_iteration(Database& database, Tools::RNG& rng);
};

} // namespace

#endif

#include "StripeImpl.h"
