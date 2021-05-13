#include "Double.h"
#include <Tools/Misc.hpp>

namespace Forklift
{

Double::Double(double _value, double _tiebreaker)
:value(_value)
,tiebreaker(_tiebreaker)
{

}

Double::Double(double _value, Tools::RNG& rng)
:value(_value)
,tiebreaker(rng.rand())
{

}

Double::Double(double _value, double _tiebreaker, Tools::RNG& rng)
:value(_value)
,tiebreaker(_tiebreaker)
{
    tiebreaker += rng.randh();
    Tools::wrap(tiebreaker);
}


double Double::get_value() const
{
    return value;
}

double Double::get_tiebreaker() const
{
    return tiebreaker;
}

bool Double::operator < (const Double& other) const
{
    if(value < other.value)
        return true;
    if(value == other.value && tiebreaker < other.tiebreaker)
        return true;
    return false;
}

bool Double::operator >= (const Double& other) const
{
    return !(*this < other);
}


} // namespace
