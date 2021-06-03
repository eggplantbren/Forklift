#ifndef Forklift_StripeImpl_h
#define Forklift_StripeImpl_h

#include "Config.h"
#include <iostream>
#include <Tools/Misc.hpp>

namespace Forklift
{

template<Model M>
std::mutex Stripe<M>::stdout_mutex;

template<Model M>
Stripe<M>::Stripe
    (int _stripe_id,
     const std::vector<M>& _particles,
     const std::vector<Double>& _xs,
     const std::vector<Double>& _ys,
     const Double& _xstar)
:stripe_id(_stripe_id)
,particles(_particles)
,xs(_xs)
,ys(_ys)
,xstar(_xstar)
,ystar{Tools::minus_infinity, 0.0}
,iteration(0)
{
    stdout_mutex.lock();
    std::cout << "Initialising stripe " << stripe_id << ' ';
    std::cout << "with threshold " << xstar.get_value() << '.' << std::endl;
    stdout_mutex.unlock();
}

template<Model M>
void Stripe<M>::ns_iteration(Database& database, Tools::RNG& rng)
{
    ++iteration;

    // Find worst particle
    int worst = 0;
    for(int i=1; i<int(particles.size()); ++i)
    {
        if(ys[i] < ys[worst])
            worst = i;
    }

    // Save worst particle
    std::optional<std::vector<unsigned char>> bytes;
    if(rng.rand() <= Config::thin)
        bytes = particles[worst].to_bytes();

    database.save_particle(stripe_id, iteration, bytes,
                           xs[worst], ys[worst]);

    // Update threshold
    ystar = ys[worst];

    stdout_mutex.lock();
    std::cout << "Stripe " << stripe_id << ", iteration " << iteration;
    std::cout << '/' << Config::num_particles*std::get<1>(Config::depth_nats);
    std::cout << " (depth ~= " << double(iteration)/Config::num_particles
              << " nats)";
    std::cout << ':' << std::endl;

    std::cout << "Saved particle. Threshold updated to (";
    std::cout << xstar.get_value() << ", " << ystar.get_value();
    std::cout << ").\n" << std::endl;
    stdout_mutex.unlock();

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
    for(int i=0; i<Config::mcmc_steps; ++i)
    {
        M proposal = particles[k];
        double logh = proposal.perturb(rng);

        if(rng.rand() <= exp(logh))
        {
            Double x = {proposal.x(), xs[k].get_tiebreaker(), rng};
            Double y = {proposal.y(), ys[k].get_tiebreaker(), rng};
            if(x >= xstar && y >= ystar)
            {
                particles[k] = proposal;
                xs[k] = x;
                ys[k] = y;
                ++accepted;
            }
        }
    }

    stdout_mutex.lock();
    std::cout << "Stripe " << stripe_id << ", iteration " << iteration;
    std::cout << '/' << Config::num_particles*std::get<1>(Config::depth_nats);
    std::cout << " (depth ~= " << double(iteration)/Config::num_particles
              << " nats)";
    std::cout << ":\n";
    std::cout << "Accepted " << accepted << '/' << Config::mcmc_steps << ' ';
    std::cout << "proposals.\n" << std::endl;
    stdout_mutex.unlock();
}

} // namespace

#endif
