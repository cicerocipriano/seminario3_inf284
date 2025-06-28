#ifndef COMPUTER_H
#define COMPUTER_H

#include "market.h"

using ul = unsigned long;

struct computer{
    inline computer() : _events_sum(0) {};
    inline computer(const ul n_exchanges): _events_sum(0) {this->_constraint_count.resize(n_exchanges, 0);};
    
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

    inline void gen_cc(std::unordered_map<ul, ul> &cc){this->_constraint_count.resize(cc.size(), 0);};

    std::vector<market> _markets;
    std::vector<ul> _constraint_count;
    ul _events_sum;
};

#endif // COMPUTER_H