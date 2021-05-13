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
     const Double& _xstar)
:stripe_id(_stripe_id)
,particles(_particles)
,xstar(_xstar)
,ystar(Tools::minus_infinity)
,iteration(0)
{
    std::cout << "Initialising stripe " << stripe_id << ' ';
    std::cout << "with threshold " << xstar.get_value() << "..." << std::flush;

    xs.reserve(particles.size());
    ys.reserve(particles.size());
    for(const auto& m: particles)
    {
        xs.emplace_back(m.x());
        ys.emplace_back(m.y());
    }

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
    std::optional<std::vector<unsigned char>> bytes;
    if(rng.rand() <= Constants::thin)
        bytes = particles[worst].to_bytes();

    database.save_particle(stripe_id, iteration, bytes,
                           xs[worst].get_value(), ys[worst].get_value());

    // Update threshold
    ystar = ys[worst];

    std::cout << "Saved particle. Threshold updated to (";
    std::cout << xstar.get_value() << ", " << ystar.get_value();
    std::cout << ")." << std::endl;

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
                Double y = {proposal.y(), ys[k].get_tiebreaker(), rng};
                if(y >= ystar)
                {
                    particles[k] = proposal;
                    xs[k] = x;
                    ys[k] = y;
                    ++accepted;
                }
            }
        }
    }

    std::cout << "done. ";
    std::cout << "Accepted " << accepted << '/' << Constants::mcmc_steps << ' ';
    std::cout << "proposals.\n" << std::endl;
}

} // namespace

#endif
