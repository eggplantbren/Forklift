#ifndef Forklift_StripeImpl_h
#define Forklift_StripeImpl_h

#include <Constants.h>
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
    std::cout << "done.\n" << std::endl;
}

template<Model M>
void Stripe<M>::ns_iteration(Database& database, Tools::RNG& rng)
{
    ++iteration;

    std::cout << "Stripe " << stripe_id << ", iteration " << iteration;
    std::cout << ':' << std::endl;

    // Find worst particle
    int worst = 0;
    for(int i=1; i<int(particles.size()); ++i)
    {
        if(ys[i] < ys[worst])
            worst = i;
    }

    // Save worst particle
    database.save_particle(stripe_id, iteration, particles[worst].to_bytes(),
                           particles[worst].x(), ys[worst]);

    // Update threshold
    ystar = ys[worst];

    std::cout << "Saved particle. Threshold updated to (";
    std::cout << xstar << ", " << ystar << ")." << std::endl;

    std::cout << "Replacing particle..." << std::flush;

    // Clone survivor
    if(particles.size() > 1)
    {
        int copy;
        while(true)
        {
            copy = rng.rand_int(particles.size());
            if(copy != worst)
                break;
        }

        particles[worst] = particles[copy];
        ys[worst] = ys[copy];
    }

    // Do MCMC
    int& k = worst;

    int accepted = 0;
    for(int i=0; i<Constants::mcmc_steps; ++i)
    {
        M proposal = particles[k];
        double logh = proposal.perturb(rng);

        if(rng.rand() <= exp(logh))
        {
            double x = proposal.x();
            if(x >= xstar)
            {
                double y = proposal.y();
                if(y >= ystar)
                {
                    particles[k] = proposal;
                    ys[k] = y;
                    ++accepted;
                }
            }
        }
    }

    std::cout << "done. ";
    std::cout << "Accepted " << accepted << '/' << Constants::mcmc_steps << ' ';
    std::cout << "proposals." << std::endl;
    std::cout << "EXITING DUE TO EXIT(0) IN CODE." << std::endl;
    exit(0);
}

} // namespace

#endif
