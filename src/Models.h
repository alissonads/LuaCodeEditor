//
// Created by Opala Studios on 21/11/23.
//

#ifndef LUACODEEDITOR_MODELS_H
#define LUACODEEDITOR_MODELS_H

#include <cstring>

using longl = long long;

enum class Interval {
    Interval_1Minute,
    Interval_3Minutes,
    Interval_5Minutes,
    Interval_15Minutes,
    Interval_30Minutes,
    Interval_1Hour,
    Interval_2Hours,
    Interval_4Hours,
    Interval_6Hours,
    Interval_8Hours,
    Interval_12Hours,
    Interval_1Day,
    Interval_3Days,
    Interval_1Week,
    Interval_1Month,
    Interval_Unknown
};

struct TimeValue {
    double value;
    longl time;

    //the operators are needed for upper_bound and lower_bound operations
    bool operator == (TimeValue const &obj) const{
        return time == obj.time;
    }

    bool operator >= (TimeValue const &obj) const{
        return time >= obj.time;
    }

    bool operator > (TimeValue const &obj) const{
        return time > obj.time;
    }

    bool operator <= (TimeValue const &obj) const  {
        return time <= obj.time;
    }

    bool operator < (TimeValue const &obj) const {
        return time < obj.time;
    }
};

struct TickData: public TimeValue{
    double volume;
    TickData()    { Clear(); }
    void Clear()    { memset(this, 0, sizeof(*this)); }
};


#endif //LUACODEEDITOR_MODELS_H
