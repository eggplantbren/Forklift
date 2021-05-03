#include <iostream>
#include <Constants.h>
#include <Example.h>
#include <Sampler.h>

using namespace Forklift;

int main()
{
    Sampler<Example> sampler{Tools::RNG()};

    for(int i=0; i<Constants::depth_nats; ++i)
        sampler.update();

    return 0;
}

