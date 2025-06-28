#ifndef COMPUTER_H
#define COMPUTER_H

#include "market.h"

using ul = unsigned long;

struct computer{
    inline computer(): _events_sum(0) {};
    
    inline ~computer(){};

    inline void print() const{
        for(auto &i: this->_markets)
            i.print();
        printf("Sum of events: %lu\nNumber of markets: %lu\n", this->_events_sum, this->_markets.size());
    };

    inline ul calc_sum(){
        ul sum = 0;
        for(auto &i: this->_markets)
            sum += i._events_avg;
        this->_events_sum = sum;
        return sum;
    };

    std::vector<market> _markets;
    ul _events_sum;
};

#endif // COMPUTER_H