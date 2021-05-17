#ifndef Forklift_Examples_Demo_hpp
#define Forklift_Examples_Demo_hpp

#include <Tools/Misc.hpp>
#include <Tools/RNG.hpp>
#include <vector>

namespace Forklift
{

namespace Examples
{

class Demo
{
    private:
        static constexpr int size = 100;
        std::vector<double> params;

    public:
        Demo() = delete;
        Demo(Tools::RNG& rng);
        Demo(std::vector<unsigned char> bytes);
        double perturb(Tools::RNG& rng);
        double x() const;
        double y() const;
        std::vector<unsigned char> to_bytes() const;
};

/* IMPLEMENTATIONS FOLLOW */

Demo::Demo(Tools::RNG& rng)
:params(size)
{
    for(double& value: params)
        value = rng.rand();
}

Demo::Demo(std::vector<unsigned char> bytes)
:params(size)
{
    assert(sizeof(double)*size == bytes.size());
    std::memcpy(&params[0], &bytes[0], bytes.size());
}

double Demo::perturb(Tools::RNG& rng)
{
    int k = rng.rand_int(size);
    params[k] += rng.randh();
    Tools::wrap(params[k]);
    return 0.0;
}

double Demo::x() const
{
    double f = 0.0;
    for(double value: params)
        f += -1E2*pow(value - 0.5, 2);
    return f;
}

double Demo::y() const
{
    double g = 0.0;
    for(double value: params)
        g += -pow(sin(10.0*M_PI*value), 2);
    return g;
}


std::vector<unsigned char> Demo::to_bytes() const
{
    std::vector<unsigned char> bytes(size*sizeof(double));
    std::memcpy(&bytes[0], &params[0], bytes.size());
    return bytes;
}


} // namespace Examples

} // namespace

#endif

