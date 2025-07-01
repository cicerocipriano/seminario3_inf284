#ifndef MARKET_H
#define MARKET_H

#include "json.hpp"

using ul = unsigned long;

struct market {
  inline market() : _events_avg(0), _exchange_id(0), _market_id(0){};

  inline market(const ul events_avg, const ul exchange_id, const ul market_id)
      : _events_avg(events_avg), _exchange_id(exchange_id),
        _market_id(market_id){};

  inline ~market(){};

  inline void print() const {
    printf("exchange_id: %lu, market_id: %lu, events_avg: %lu\n",
           this->_exchange_id, this->_market_id, this->_events_avg);
    
    
  };

  ul _events_avg, _exchange_id, _market_id;
};

#endif // SOLUTION_H