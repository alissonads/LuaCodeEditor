//
// Created by Opala Studios on 21/11/23.
//

#ifndef LUACODEEDITOR_INTERVALPARSER_H
#define LUACODEEDITOR_INTERVALPARSER_H

#include <string>
#include "Models.h"

class IntervalParser {
public:
    static Interval stringToInterval(const std::string &);

    static std::string intervalToString(const Interval &interval);

    static longl getTimeIntervalInMinutes(const Interval &interval);

    static longl getTimeIntervalInSeconds(const Interval &interval);

    static longl getTimeIntervalInMilliSeconds(const Interval &interval);

private:
    inline static std::vector<longl> _arrMinutes = {
            1,
            3,
            5,
            15,
            30,
            60,
            60 * 2,
            60 * 4,
            60 * 6,
            60 * 8,
            60 * 12,
            60 * 24,
            60 * 24 * 3,
            60 * 24 * 7,
            60 * 24 * 30
    };
};


#endif //LUACODEEDITOR_INTERVALPARSER_H
