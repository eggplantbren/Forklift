#ifndef Forklift_StripeImpl_h
#define Forklift_StripeImpl_h

#include <iostream>
#include <Tools/Misc.hpp>

namespace Forklift
{

template<Model M>
Stripe<M>::Stripe(int _stripe_id, const std::vector<M>& _particles,
                  double _xstar)
:stripe_id(_stripe_id)
,particles(_particles)
,xstar(_xstar)
,ystar(Tools::minus_infinity)
{
    std::cout << "Initialised stripe " << stripe_id << ' ';
    std::cout << "with threshold " << xstar << '.' << std::endl;
}



} // namespace

#endif
