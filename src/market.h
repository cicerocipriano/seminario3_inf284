#ifndef MARKET_H
#define MARKET_H

#include "json.hpp"

using ul = unsigned long;

struct market{
    // constructors
    market(const ul, const ul, const ul);

    // destructor
    ~market();

    // methods
    inline void print(nlohmann::json &) const;
    inline void print_raw() const;

    // attributes
    ul _events_avg, _market_id, _exchange_id;
};

#endif // SOLUTION_H