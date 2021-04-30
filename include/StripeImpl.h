#ifndef Forklift_StripeImpl_h
#define Forklift_StripeImpl_h

#include <iostream>
#include <Tools/Misc.hpp>

namespace Forklift
{

template<Model M>
Stripe<M>::Stripe
    (int _stripe_id,
     const std::vector<M>& _particles,
     double _xstar)
:stripe_id(_stripe_id)
,particles(_particles)
,xstar(_xstar)
,ystar(Tools::minus_infinity)
,iteration(0)
{
    std::cout << "Initialising stripe " << stripe_id << ' ';
    std::cout << "with threshold " << xstar << "..." << std::flush;
    ys.reserve(particles.size());
    for(const auto& m: particles)
        ys.emplace_back(m.y());
    std::cout << "done." << std::endl;
}

template<Model M>
void Stripe<M>::ns_iteration(Database& database, Tools::RNG& rng)
{
    ++iteration;

    // Find worst particle
    int worst = 0;
    for(int i=1; i<int(particles.size()); ++i)
    {
        if(ys[i] < worst)
            worst = i;
    }

    // Save worst particle
    database.save_particle(stripe_id, iteration, particles[worst].to_bytes(),
                           particles[worst].x(), ys[worst]);

    std::cout << "EXITING DUE TO EXIT(0) IN CODE." << std::endl;
    exit(0);
}

} // namespace

#endif
