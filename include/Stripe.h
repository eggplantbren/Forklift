#ifndef Forklift_Stripe_h
#define Forklift_Stripe_h

#include "Model.h"

namespace Forklift
{

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

        Stripe(int _stripe_id, const std::vector<M>& _particles,
               double _xstar);


};

} // namespace

#endif

#include "StripeImpl.h"
