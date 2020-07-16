#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds)
{
    string hoursString, minutesString, secondsString;
    int remainingSeconds, minutes, remainingMinutes, hours;

    remainingSeconds = seconds % 60;
    if (remainingSeconds < 10) { secondsString = '0'+to_string(remainingSeconds); }
    else                       { secondsString =     to_string(remainingSeconds); }

    minutes = seconds / 60;
    remainingMinutes = minutes % 60;
    if (remainingMinutes < 10) { minutesString = '0'+to_string(remainingMinutes); }
    else                       { minutesString =     to_string(remainingMinutes); }

    hours = minutes / 60;
    if (hours < 10) { hoursString = '0'+to_string(hours); }
    else            { hoursString =     to_string(hours); }

    return hoursString+':'+minutesString+':'+secondsString;
}