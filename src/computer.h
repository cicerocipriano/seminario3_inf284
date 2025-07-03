#ifndef COMPUTER_H
#define COMPUTER_H

#include "market.h"

using ul = unsigned long;
using ld = long double;

struct computer {
  inline computer() : _events_sum(0){};

  inline computer(const ul n_exchanges) : _events_sum(0) {
    this->_constraint_count.resize(n_exchanges, 0);
  };

  inline ~computer(){};

  inline void print() const {
    for (const market &i : this->_markets)
      i.print();
    printf("Sum of events: %lu\nNumber of markets: %lu\nComputer Distribution: "
           "%lu\n",
           this->_events_sum, this->_markets.size(),
           this->_computer_distribution);
  };

  inline ul calc_sum() {
    ul sum = 0;
    for (const market &i : this->_markets)
      sum += i._events_avg;
    this->_events_sum = sum, this->_events_sum_square = pow(sum, 2);
    return sum;
  };

  inline void gen_cc(std::unordered_map<ul, ul> &cc) {
    this->_constraint_count.resize(cc.size(), 0);
  };

  inline bool
  check_computer(std::unordered_map<ul, ul> &hard_constraints) const {
    for (ul i = 0; i < this->_constraint_count.size(); ++i)
      if (this->_constraint_count[i] > hard_constraints.at(i))
        return false;
    return true;
  };

  inline ld calc_cd(std::unordered_map<ul, ul> &hard_constraints) {
    std::vector<ul> vec(this->_constraint_count.size());
    for (std::pair<ul, ul> i : hard_constraints)
      vec[i.first] = (100.0 * this->_constraint_count[i.first]) / i.second;
    ld sum = 0.0, mean, variance;
    for (ul i : vec)
      sum += i;
    mean = sum / vec.size(), sum = 0.0;
    for (ul i : vec)
      sum += pow(i - mean, 2);
    variance = sum / vec.size();
    this->_computer_distribution = sqrt(variance);
    return this->_computer_distribution;
  };

  std::vector<market> _markets;
  std::vector<ul> _constraint_count;
  ul _events_sum, _events_sum_square;
  ld _computer_distribution;
};

#endif // COMPUTER_H