#include "computer.h"
#include <cstdio>

using ul = unsigned long;
using namespace std;

computer::computer(): _events_sum(0) {}

computer::~computer(){}

// methods
void computer::print(nlohmann::json &) const{}

void computer::print_raw() const{
    for(auto &i: this->_markets)
        i.print_raw();
    printf("Sum of events: %lu\nNumber of markets: %lu\n", this->_events_sum, this->_markets.size());
}

ul computer::calc_sum(){
    ul sum = 0;
    for(auto &i: this->_markets)
        sum += i._events_avg;
    this->_events_sum = sum;
    return sum;
}