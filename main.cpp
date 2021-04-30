#include <iostream>
#include <Example.h>
#include <Sampler.h>

using namespace Forklift;

int main()
{
    Sampler<Example> sampler{Tools::RNG()};
    sampler.update();

    return 0;
}

