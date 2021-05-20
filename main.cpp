#include <iostream>
#include "Config.h"
#include <Examples/Demo.hpp>
#include <Sampler.h>

using namespace Forklift;

int main()
{
    Config::load("config.yaml");
    Sampler<Examples::Demo> sampler{Tools::RNG(Config::rng_seed)};
    for(int i=0; i<Config::depth_nats; ++i)
        sampler.update();

    return 0;
}

