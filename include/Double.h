#ifndef Forklift_Double_h
#define Forklift_Double_h

#include <Tools/RNG.hpp>

namespace Forklift
{

// A double with a tiebreaker
class Double
{
    private:
        double value;
        double tiebreaker;

    public:
        Double(double _value, double _tiebreaker);
        bool operator < (const Double& other) const;
};

} // namespace

#endif
