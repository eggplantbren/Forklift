#include "Double.h"

namespace Forklift
{

Double::Double(double _value, double _tiebreaker)
:value(_value)
,tiebreaker(_tiebreaker)
{

}

bool Double::operator < (const Double& other) const
{
    if(value < other.value)
        return true;
    if(value == other.value && tiebreaker < other.tiebreaker)
        return true;
    return false;
}

} // namespace
