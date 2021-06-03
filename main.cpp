#include <iostream>
#include "Config.h"
#include <Examples/All.hpp>
#include <Sampler.h>

using namespace Forklift;

int main()
{
    Config::load("config.yaml");
    Sampler<Examples::Demo> sampler;
    for(int i=0; i<std::get<0>(Config::depth_nats); ++i)
        sampler.update();

    return 0;
}

