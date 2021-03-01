#pragma once
namespace utils
{
    double toTwoDecimalPlaces(double d) // TODO move to utils or similar
    {
        int i;
        if (d >= 0)
            i = static_cast<int>(d * 100 + 0.5);
        else
            i = static_cast<int>(d * 100 - 0.5);
        return (i / 100.0);
    }

} // namespace utils
