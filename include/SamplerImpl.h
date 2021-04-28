#ifndef Forklift_SamplerImpl_h
#define Forklift_SamplerImpl_h

#include "Constants.h"
#include <iostream>

namespace Forklift
{

template<Model M>
Sampler<M>::Sampler(Tools::RNG&& _rng)
:rng(_rng)
{
    std::cout << "Generating particles from the prior..." << std::flush;

    particles.reserve(Constants::num_particles);
    scalars.reserve(Constants::num_particles);
    for(int i=0; i<Constants::num_particles; ++i)
    {
        M m(rng);
        scalars.emplace_back(m.scalars());
        particles.emplace_back(std::move(m));
    }

    std::cout << "done." << std::endl;
}

} // namespace

#endif

