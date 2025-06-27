#ifndef COMPUTER_H
#define COMPUTER_H

#include "market.h"

using ul = unsigned long;

struct computer{
    computer();
    ~computer();

    inline void print() const;
    inline ul calc_sum();

    std::vector<market> _markets;
    ul _events_sum;
};

#endif // COMPUTER_H