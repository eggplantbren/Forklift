#include <iostream>
#include <Example.h>
#include <Sampler.h>

using namespace Forklift;

int main()
{
    Sampler<Example> sampler{Tools::RNG()};
    sampler.run_to_depth(100);

    return 0;
}

