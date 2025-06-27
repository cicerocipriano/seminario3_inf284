#ifndef SOLUTION_H
#define SOLUTION_H

#include "computer.h"

using ul = unsigned long;
using ld = long double;

struct solution{
    solution();
    ~solution();

    inline void print() const;
    inline void calc_mean();
    inline void calc_variance();
    inline void calc_std_deviation();
    inline void evaluation();
    inline ld partial_eval() const;

    std::unordered_map<ul, computer> _computers;
    ld _mean, _variance, _std_deviation, _value;
};  

#endif // SOLUTION_H