#include "market.h"
#include <cstdio>

using ul = unsigned long;

market::market(const ul events_avg, const ul exchange_id, const ul market_id) : _events_avg(events_avg), _exchange_id(exchange_id), _market_id(market_id) {}

market::~market() {}

void market::print() const {printf("exchange_id: %lu, market_id: %lu, events_avg: %lu\n", this->_exchange_id, this->_market_id, this->_events_avg);}