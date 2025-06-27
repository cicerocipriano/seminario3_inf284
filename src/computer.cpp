#include "computer.h"
#include <cstdio>

using ul = unsigned long;

computer::computer(): _events_sum(0) {}

computer::~computer(){}

void computer::print() const{
    for(auto &i: this->_markets)
        i.print();
    printf("Sum of events: %lu\nNumber of markets: %lu\n", this->_events_sum, this->_markets.size());
}

ul computer::calc_sum(){
    ul sum = 0;
    for(auto &i: this->_markets)
        sum += i._events_avg;
    this->_events_sum = sum;
    return sum;
}