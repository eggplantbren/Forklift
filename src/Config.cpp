#include "Config.h"
#include <cassert>
#include <yaml-cpp/yaml.h>

namespace Forklift
{

int Config::num_particles_floor = 100;
int Config::num_particles_stripe = 30;
int Config::mcmc_steps = 1000;
std::pair<int, int> Config::depth_nats = {100, 100};
double Config::thin = 0.1;
int Config::rng_seed = 0;
int Config::num_threads = 4;

void Config::load(const char* filename)
{
    YAML::Node node = YAML::LoadFile(filename);
    num_particles_floor = node["num_particles_floor"].as<int>();
    num_particles_stripe = node["num_particles_stripe"].as<int>();
    assert(num_particles_floor >= num_particles_stripe);

    mcmc_steps = node["mcmc_steps"].as<int>();
    depth_nats = {node["depth_nats"][0].as<int>(),
                  node["depth_nats"][1].as<int>()};
    thin = node["thin"].as<double>();

    try
    {
        rng_seed = node["rng_seed"].as<int>();
    }
    catch(...)
    {
        rng_seed = time(0);
    }

    num_threads = node["num_threads"].as<int>();
}

} // namespace
