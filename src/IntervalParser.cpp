//
// Created by Opala Studios on 21/11/23.
//

#include "IntervalParser.h"

Interval IntervalParser::stringToInterval(const std::string& value) {
    if ( value == "1m" ) return Interval::Interval_1Minute;
    if ( value == "3m" ) return Interval::Interval_3Minutes;
    if ( value == "5m" ) return Interval::Interval_5Minutes;
    if ( value == "15m" ) return Interval::Interval_15Minutes;
    if ( value == "30m" ) return Interval::Interval_30Minutes;
    if ( value == "1h" ) return Interval::Interval_1Hour;
    if ( value == "2h" ) return Interval::Interval_2Hours;
    if ( value == "4h" ) return Interval::Interval_4Hours;
    if ( value == "6h" ) return Interval::Interval_6Hours;
    if ( value == "8h" ) return Interval::Interval_8Hours;
    if ( value == "12h" ) return Interval::Interval_12Hours;
    if ( value == "1d" ) return Interval::Interval_1Day;
    if ( value == "3d" ) return Interval::Interval_3Days;
    if ( value == "1w" ) return Interval::Interval_1Week;
    if ( value == "1mo" ) return Interval::Interval_1Month;
    else return Interval::Interval_Unknown;
}

std::string IntervalParser::intervalToString(const Interval& interval) {
    switch (interval) {
        case Interval::Interval_1Minute:    return "1m";
        case Interval::Interval_3Minutes:   return "3m";
        case Interval::Interval_5Minutes:   return "5m";
        case Interval::Interval_15Minutes:  return "15m";
        case Interval::Interval_30Minutes:  return "30m";
        case Interval::Interval_1Hour:      return "1h";
        case Interval::Interval_2Hours:      return "2h";
        case Interval::Interval_4Hours:      return "4h";
        case Interval::Interval_6Hours:      return "6h";
        case Interval::Interval_8Hours:      return "8h";
        case Interval::Interval_12Hours:     return "12h";
        case Interval::Interval_1Day:       return "1d";
        case Interval::Interval_3Days:      return "3d";
        case Interval::Interval_1Week:      return "1w";
        case Interval::Interval_1Month:     return "1M";
        case Interval::Interval_Unknown:
        default:
            return "";
    }
}

longl IntervalParser::getTimeIntervalInMinutes(const Interval & interval) {
    return _arrMinutes[longl(interval)];
}

longl IntervalParser::getTimeIntervalInSeconds(const Interval &interval){
    return _arrMinutes[longl(interval)]*60;
}

longl IntervalParser::getTimeIntervalInMilliSeconds(const Interval &interval){
    return _arrMinutes[longl(interval)]*60*1000;
}