#ifndef Forklift_Sampler_h
#define Forklift_Sampler_h

#include <Model.h>

namespace Forklift
{

/*
* A Sampler is a single run of Nested Sampling,
* either to produce the floor or a stripe.
*/

template<Model M>
class Sampler
{
    private:

        // Random number generator
        Tools::RNG rng;

        // Particles and their scalars
        std::vector<M> particles;
        std::vector<std::vector<double>> scalars;

    public:

        // Construct with a random number generator
        Sampler(Tools::RNG&& _rng);
        Sampler() = delete;
};

} // namespace

#endif

#include "SamplerImpl.h"
