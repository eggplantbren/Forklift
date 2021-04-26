#ifndef Forklift_Example_h
#define Forklift_Example_h

#include <Tools/RNG.hpp>
#include <vector>

namespace Forklift
{

class Example
{
    private:
        static constexpr int size = 20;
        std::vector<double> params;

    public:
        Example() = delete;
        Example(Tools::RNG& rng);
        Example(std::vector<unsigned char>&& bytes);
        double perturb(Tools::RNG& rng);
        std::vector<double> scalars() const;
        std::vector<unsigned char> to_bytes() const;
};

} // namespace

#endif

