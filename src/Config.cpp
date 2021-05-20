#include "Config.h"
#include <yaml-cpp/yaml.h>

namespace Forklift
{

int Config::num_particles = 30;
int Config::mcmc_steps = 1000;
int Config::depth_nats = 100;
double Config::thin = 0.1;

void Config::load(const char* filename)
{
    YAML::Node node = YAML::LoadFile(filename);
    num_particles = node["num_particles"].as<int>();
    mcmc_steps = node["mcmc_steps"].as<int>();
    depth_nats = node["depth_nats"].as<int>();
    thin = node["thin"].as<double>();
}

} // namespace
