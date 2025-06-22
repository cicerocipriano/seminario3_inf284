#include "solution.h"
#include <cmath>
#include <cstdio>

using ul = unsigned long;
using ld = long double;
using namespace std;

solution::solution(): _mean(0.0), _variance(0.0), _std_deviation(0.0) {}

solution::~solution(){}

void solution::print(nlohmann::json &j) const {}

void solution::print_raw() const {
    for (const auto &i : this->_computers)
        i.second.print_raw();
    printf("Number of computers: %lu\n", this->_computers.size());
    printf("Mean: %.2Lf\n", this->_mean);
    printf("Variance: %.2Lf\n", this->_variance);
    printf("Standard Deviation: %.2Lf\n", this->_std_deviation);   
}

ld solution::calc_mean() {
    if(this->_computers.empty()) return 0.0;
    ld sum = 0.0;
    for (const auto &i : this->_computers)
        sum += i.second._events_sum;
    this->_mean = sum / this->_computers.size();
    return this->_mean;
}

ld solution::calc_variance() {
    return this->_variance;
}

ld solution::calc_std_deviation() {
    return this->_std_deviation;
}

