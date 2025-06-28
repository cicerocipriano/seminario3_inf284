#ifndef SOLUTION_H
#define SOLUTION_H

#include "computer.h"

using ul = unsigned long;
using ld = long double;

struct solution{
    inline solution(): _mean(0.0), _variance(0.0), _std_deviation(0.0), _value(0.0) {};

    inline ~solution(){};

    inline void print() const{
        for (const auto &i : this->_computers)
            i.second.print();
        printf("Number of computers: %lu\nMean: %.5Lf\nVariance: %.5Lf\nStandard Deviation: %.5Lf\nSolution Value: %.5Lf\n", this->_computers.size(), this->_mean, this->_variance, this->_std_deviation, this->_value);
    };

    inline void calc_mean(){
        ld sum = 0.0;
        for (const auto &i : this->_computers)
            sum += i.second._events_sum;
        this->_mean = sum / this->_computers.size();
    };

    inline void calc_variance(){
        ld sum = 0.0;
        for (const auto &i : this->_computers)
            sum += pow(i.second._events_sum - this->_mean, 2);
        this->_variance = sum / this->_computers.size();
    };

    inline void calc_std_deviation(){this->_std_deviation = sqrt(this->_variance);};

    inline void evaluation(){
        if(this->_computers.empty()){
            this->_mean = this->_variance = this->_std_deviation = this->_value = -1.0;
            return;
        }
        this->calc_mean(), this->calc_variance(), this->calc_std_deviation(), this->_value = this->_std_deviation;
    };

    inline ld partial_eval() const{return 0.0;};

    std::unordered_map<ul, computer> _computers;
    ld _mean, _variance, _std_deviation, _value;
};  

#endif // SOLUTION_H