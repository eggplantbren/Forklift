#ifndef Forklift_Model_h
#define Forklift_Model_h

#include <Tools/RNG.hpp>
#include <vector>

namespace Forklift
{

/*
* Define the Model concept
*/

template<typename M>
concept Model = requires(Tools::RNG& rng)
{
    { M{rng} };
}
&& requires(std::vector<unsigned char> bytes)
{
    { M{bytes} };
}
&& requires(const M& m)
{
    { m.x() } -> std::convertible_to<double>;
    { m.y() } -> std::convertible_to<double>;
    { m.to_bytes() } -> std::convertible_to<std::vector<unsigned char>>;
}
&& requires(M& m, Tools::RNG& rng)
{
    { m.perturb(rng) } -> std::convertible_to<double>;
};



} // namespace

#endif

