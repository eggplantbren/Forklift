#ifndef Forklift_Stripe_h
#define Forklift_Stripe_h

#include "Database.h"
#include "Double.h"
#include "Model.h"
#include <mutex>
#include <Tools/RNG.hpp>
#include <vector>

namespace Forklift
{

template<Model M>
class Stripe
{
    private:

        // For cout
        static std::mutex stdout_mutex;

        // Stripe number
        int stripe_id;

        // Particles and their x and y-values
        std::vector<M> particles;
        std::vector<Double> xs, ys;

        // Threshold
        Double xstar, ystar;

        // Iteration
        int iteration;

    public:

        // No default constructor
        Stripe() = delete;

        // Constructor
        Stripe(int _stripe_id,
               const std::vector<M>& _particles,
               const std::vector<Double>& _xs,
               const std::vector<Double>& _ys,
               const Double& _xstar);

        // Do one NS iteration
        void ns_iteration(Database& database, Tools::RNG& rng);
};

} // namespace

#endif

#include "StripeImpl.h"
