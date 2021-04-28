#ifndef Forklift_SamplerImpl_h
#define Forklift_SamplerImpl_h

namespace Forklift
{

template<Model M>
Sampler<M>::Sampler(Tools::RNG&& _rng)
:rng(_rng)
{

}

} // namespace

#endif

