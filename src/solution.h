#ifndef SOLUTION_H
#define SOLUTION_H

#include "computer.h"
#include <unordered_map>

using ul = unsigned long;
using ld = long double;

struct solution{
    // constructors
    solution();

    // destructor
    ~solution();

    // methods
    inline void print(nlohmann::json &) const;
    inline void print_raw() const;
    inline ld calc_mean();
    inline ld calc_variance();
    inline ld calc_std_deviation();

    // attributes
    std::unordered_map<ul, computer> _computers;
    ld _mean, _variance, _std_deviation;

};  

#endif // SOLUTION_H