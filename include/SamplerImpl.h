#ifndef Forklift_SamplerImpl_h
#define Forklift_SamplerImpl_h

#include "Constants.h"
#include <iostream>
#include <Tools/Misc.hpp>

namespace Forklift
{

template<Model M>
Sampler<M>::Sampler(Tools::RNG&& _rng)
:rng(_rng)
,stripe_id(0)
,iteration(0)
,xstar(Tools::minus_infinity, 0.0)
{
    std::cout << "Generating particles from the prior..." << std::flush;

    particles.reserve(Constants::num_particles);
    xs.reserve(Constants::num_particles);
    ys.reserve(Constants::num_particles);
    for(int i=0; i<Constants::num_particles; ++i)
    {
        M m{rng};
        xs.emplace_back(m.x(), rng);
        ys.emplace_back(m.y(), rng);
        particles.emplace_back(std::move(m));
    }

    std::cout << "done.\n" << std::endl;
}


template<Model M>
void Sampler<M>::update()
{
    // Create stripe and ascend
    Stripe<M> stripe(stripe_id, particles, xs, ys, xstar);
    int stripe_iterations = Constants::num_particles
                                        *(Constants::depth_nats - 0); //stripe_id);
    for(int i=0; i<stripe_iterations; ++i)
        stripe.ns_iteration(database, rng);

    ++stripe_id;

    for(int i=0; i<Constants::num_particles; ++i)
        ns_iteration();

    // Clear out particles
    database.clear_above(xstar);
}



template<Model M>
void Sampler<M>::ns_iteration()
{
    ++iteration;

    std::cout << "Floor iteration " << iteration << '.' << std::endl;

    // Find worst particle
    int worst = 0;
    for(int i=1; i<int(particles.size()); ++i)
    {
        if(xs[i] < xs[worst])
            worst = i;
    }

    // Update threshold
    xstar = xs[worst];
    std::cout << "Threshold = " << xstar.get_value() << '.' << std::endl;

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
        xs[worst] = xs[copy];
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
            Double x = {proposal.x(), xs[k].get_tiebreaker(), rng};
            if(x >= xstar)
            {
                particles[k] = proposal;
                xs[k] = x;
                ys[k] = {proposal.y(), rng};
                ++accepted;
            }
        }
    }

    std::cout << "done. ";
    std::cout << "Accepted " << accepted << '/' << Constants::mcmc_steps << ' ';
    std::cout << "proposals.\n" << std::endl;
}


} // namespace

#endif

