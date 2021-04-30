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
,iteration(0)
,xstar(Tools::minus_infinity)
{
    std::cout << "Generating particles from the prior..." << std::flush;

    particles.reserve(Constants::num_particles);
    for(int i=0; i<Constants::num_particles; ++i)
        particles.emplace_back(M{rng});

    std::cout << "done." << std::endl;
}


/*template<Model M>*/
/*void Sampler<M>::run_to_depth(int nats)*/
/*{*/
/*    int iterations = nats*Constants::num_particles;*/
/*    for(int i=0; i<iterations; ++i)*/
/*        update();*/
/*}*/


template<Model M>
void Sampler<M>::update()
{
    Stripe<M> stripe(iteration, particles, xstar);


    ++iteration;

/*    // Which scalar to ascend*/
/*    int which_scalar = (stripe.size() == 0)?(0):(1);*/

/*    // Find worst particle*/
/*    int worst = 0;*/
/*    for(int i=1; i<Constants::num_particles; ++i)*/
/*    {*/
/*        if(scalars[i][which_scalar] < scalars[worst][which_scalar])*/
/*            worst = i;*/
/*    }*/

/*    // Save it*/
/*    database.save_particle(particles[worst].to_bytes(),*/
/*                           scalars[worst][0], scalars[worst][1]);*/
}


} // namespace

#endif

