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

Example::Example(std::vector<unsigned char>&& bytes)
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

std::vector<double> Example::scalars() const
{
    double x = 0.0;
    double y = 0.0;

    for(double value: params)
    {
        x += -0.5*1E4*pow(value - 0.5, 2);
        y += -1E2*value;
    }

    return {x, y};
}

std::vector<unsigned char> Example::to_bytes() const
{
    std::vector<unsigned char> bytes(size*sizeof(double));
    std::memcpy(&bytes[0], &params[0], bytes.size());
    return bytes;
}

} // namespace

