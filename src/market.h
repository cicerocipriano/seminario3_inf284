#ifndef MARKET_H
#define MARKET_H

#include "json.hpp"

using ul = unsigned long;

struct market{
    market(const ul, const ul, const ul);
    ~market();

    inline void print() const;

    ul _events_avg, _exchange_id, _market_id;
};

#endif // SOLUTION_H