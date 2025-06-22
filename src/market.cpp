#include "market.h"
#include <cstdio>

using ul = unsigned long;
using namespace std;

market::market(const ul events_avg, const ul market_id, const ul exchange_id)
    : _events_avg(events_avg), _market_id(market_id), _exchange_id(exchange_id) {}

market::~market() {}

// methods
void market::print(nlohmann::json &j) const {}

void market::print_raw() const {
    printf("market_id: %lu, exchange_id: %lu, events_avg: %lu\n", this->_market_id, this->_exchange_id, this->_events_avg);
}