#ifndef COMPUTER_H
#define COMPUTER_H

#include "market.h"
#include <vector>

using ul = unsigned long;

struct computer{
    // constructors
    computer();

    // destructor
    ~computer();

    // methods
    inline void print(nlohmann::json &) const;
    inline void print_raw() const;
    inline ul calc_sum();

    // attributes
    std::vector<market> _markets;
    ul _events_sum;
};

#endif // COMPUTER_H