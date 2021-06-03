#ifndef Forklift_SamplerImpl_h
#define Forklift_SamplerImpl_h

#include "Config.h"
#include <iostream>
#include <Tools/Misc.hpp>

namespace Forklift
{

template<Model M>
Sampler<M>::Sampler()
:stripe_id(0)
,iteration(0)
,xstar(Tools::minus_infinity, 0.0)
{
    std::cout << "Setting up RNGs..." << std::flush;
    rngs.reserve(Config::num_threads);
    int seed = time(0);
    for(int i=0; i<Config::num_threads; ++i)
    {
        rngs.emplace_back(seed);
        seed += 123;
    }
    std::cout << "done." << std::endl;

    std::cout << "Generating particles from the prior..." << std::flush;

    particles.reserve(Config::num_particles);
    xs.reserve(Config::num_particles);
    ys.reserve(Config::num_particles);
    for(int i=0; i<Config::num_particles; ++i)
    {
        M m{rngs[0]};
        xs.emplace_back(m.x(), rngs[0]);
        ys.emplace_back(m.y(), rngs[0]);
        particles.emplace_back(std::move(m));
    }

    std::cout << "done.\n" << std::endl;
}


template<Model M>
void Sampler<M>::update()
{
    // Create stripe and ascend
    Stripe<M> stripe(stripe_id, particles, xs, ys, xstar);
    int stripe_iterations = Config::num_particles*std::get<1>(Config::depth_nats);
    for(int i=0; i<stripe_iterations; ++i)
        stripe.ns_iteration(database, rngs[0]);

    ++stripe_id;

    for(int i=0; i<Config::num_particles; ++i)
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
            copy = rngs[0].rand_int(particles.size());
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
    for(int i=0; i<Config::mcmc_steps; ++i)
    {
        M proposal = particles[k];
        double logh = proposal.perturb(rngs[0]);

        if(rngs[0].rand() <= exp(logh))
        {
            Double x = {proposal.x(), xs[k].get_tiebreaker(), rngs[0]};
            Double y = {proposal.y(), ys[k].get_tiebreaker(), rngs[0]};
            if(x >= xstar)
            {
                particles[k] = proposal;
                xs[k] = x;
                ys[k] = y;
                ++accepted;
            }
        }
    }

    std::cout << "done. ";
    std::cout << "Accepted " << accepted << '/' << Config::mcmc_steps << ' ';
    std::cout << "proposals.\n" << std::endl;
}


} // namespace

#endif

