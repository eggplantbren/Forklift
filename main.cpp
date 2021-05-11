#include <iostream>
#include <Constants.h>
#include <Examples/Demo.hpp>
#include <Sampler.h>

using namespace Forklift;

int main()
{
    Sampler<Examples::Demo> sampler{Tools::RNG()};

    for(int i=0; i<Constants::depth_nats; ++i)
        sampler.update();

    return 0;
}

