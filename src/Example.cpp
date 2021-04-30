#include "Example.h"
#include <cassert>
#include <cmath>
#include <memory>
#include <Tools/Misc.hpp>

namespace Forklift
{

Example::Example(Tools::RNG& rng)
:params(size)
{
    for(double& value: params)
        value = rng.rand();
}

Example::Example(std::vector<unsigned char> bytes)
:params(size)
{
    assert(sizeof(double)*size == bytes.size());
    std::memcpy(&params[0], &bytes[0], bytes.size());
}

double Example::perturb(Tools::RNG& rng)
{
    int k = rng.rand_int(size);
    params[k] += rng.randh();
    Tools::wrap(params[k]);
    return 0.0;
}

double Example::x() const
{
    double f = 0.0;
    for(double value: params)
        f += -0.5*1E4*pow(value - 0.5, 2);
    return f;
}

double Example::y() const
{
    double g = 0.0;
    for(double value: params)
        g += -1E2*value;
    return g;
}


std::vector<unsigned char> Example::to_bytes() const
{
    std::vector<unsigned char> bytes(size*sizeof(double));
    std::memcpy(&bytes[0], &params[0], bytes.size());
    return bytes;
}

} // namespace

