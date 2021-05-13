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
        // Set the values
        Double(double _value, double _tiebreaker);

        // Set the values and perturb tiebreaker while you're at it
        Double(double _value, double _tiebreaker, Tools::RNG& rng);

        // Set the value and generate tiebreaker
        Double(double _value, Tools::RNG& rng);
        double get_value() const;
        double get_tiebreaker() const;
        bool operator <  (const Double& other) const;
        bool operator >= (const Double& other) const;
};

} // namespace

#endif
