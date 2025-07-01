#ifndef SOLUTION_H
#define SOLUTION_H

#include "computer.h"

using ul = unsigned long;
using ld = long double;

struct change {
  ul change_type, from, to, who;
  std::vector<std::pair<ul, ul>> computer_sequence;
};

struct solution {
  inline solution()
      : _mean(0.0), _variance(0.0), _std_deviation(0.0), _value(0.0){};

  inline ~solution(){};

  inline void print(const bool verbose) const {
    if (verbose)
      for (std::pair<ul, computer> i : this->_computers)
        i.second.print();
    printf("Number of computers: %lu\nMean: %.5Lf\nVariance: %.5Lf\nStandard "
           "Deviation: %.5Lf\nSolution Value: %.5Lf\n",
           this->_computers.size(), this->_mean, this->_variance,
           this->_std_deviation, this->_value);
  };

  inline ul markets_in_solution() const {
    ul sum = 0;
    for (std::pair<ul, computer> i : this->_computers)
      sum += i.second._markets.size();
    return sum;
  };

  inline void apply_change(const change c, const ld new_value) {
    this->_variance = new_value;
    this->_value = this->_std_deviation = sqrt(this->_variance);
    if (c.change_type > 0) {
      std::pair<ul, ul> firstx = c.computer_sequence[0];
      ul prev, after, prev_exchange_id, after_exchange_id, last_exchange_id;
      ul first_val =
          this->_computers.at(firstx.first)._markets[firstx.second]._events_avg;
      ul first_exchange_id = this->_computers.at(firstx.first)
                                 ._markets[firstx.second]
                                 ._exchange_id;
      for (ul a = 1; a < c.computer_sequence.size(); a++) {
        prev = this->_computers.at(c.computer_sequence[a].first)
                   ._markets[c.computer_sequence[a].second]
                   ._events_avg;
        prev_exchange_id = this->_computers.at(c.computer_sequence[a].first)
                               ._markets[c.computer_sequence[a].second]
                               ._exchange_id;
        this->_computers.at(c.computer_sequence[a].first)
            ._constraint_count[prev_exchange_id]--;
        std::swap(this->_computers.at(firstx.first)._markets[firstx.second],
                  this->_computers.at(c.computer_sequence[a].first)
                      ._markets[c.computer_sequence[a].second]);
        after = this->_computers.at(c.computer_sequence[a].first)
                    ._markets[c.computer_sequence[a].second]
                    ._events_avg;
        after_exchange_id = this->_computers.at(c.computer_sequence[a].first)
                                ._markets[c.computer_sequence[a].second]
                                ._exchange_id;
        this->_computers.at(c.computer_sequence[a].first)
            ._constraint_count[after_exchange_id]++;
        this->_computers.at(c.computer_sequence[a].first)._events_sum +=
            after - prev;
      }
      last_exchange_id = this->_computers.at(firstx.first)
                             ._markets[firstx.second]
                             ._exchange_id;
      this->_computers.at(firstx.first)._constraint_count[first_exchange_id]--;
      this->_computers.at(firstx.first)._constraint_count[last_exchange_id]++;
      this->_computers.at(firstx.first)._events_sum +=
          this->_computers.at(firstx.first)
              ._markets[firstx.second]
              ._events_avg -
          first_val;
    } else {
      market last = this->_computers.at(c.from)._markets.back(), wanted;
      std::swap(this->_computers.at(c.from)._markets[c.who], last);
      wanted = this->_computers.at(c.from)._markets.back();
      this->_computers.at(c.from)._constraint_count[wanted._exchange_id]--;
      this->_computers.at(c.to)._markets.push_back(wanted);
      this->_computers.at(c.to)._constraint_count[wanted._exchange_id]++;
      this->_computers.at(c.from)._markets.pop_back();
      this->_computers.at(c.from)._events_sum -= wanted._events_avg;
      this->_computers.at(c.to)._events_sum += wanted._events_avg;
    }
  };

  inline void calc_mean() {
    ld sum = 0.0;
    for (std::pair<ul, computer> i : this->_computers)
      sum += i.second._events_sum;
    this->_mean = sum / this->_computers.size();
  };

  inline void calc_variance() {
    ld sum = 0.0;
    for (std::pair<ul, computer> i : this->_computers)
      sum += pow(i.second._events_sum - this->_mean, 2);
    this->_variance = sum / this->_computers.size();
  };

  inline void calc_std_deviation() {
    this->_std_deviation = sqrt(this->_variance);
  };

  inline void evaluation() {
    if (this->_computers.empty()) {
      this->_mean = this->_variance = this->_std_deviation = this->_value =
          -1.0;
      return;
    }
    this->calc_mean(), this->calc_variance(), this->calc_std_deviation();
    this->_value = this->_std_deviation;
  };

  inline ld partial_eval(const change &c) const {
    ul sum_changed = 0, sum_remv = 0, next = 0, new_val, old_val, sc;
    ul sumx2, new_sum, new_var, move_val, remove_comp, add_comp;
    if (c.change_type > 0) {
      for (ul a = 0; a < c.computer_sequence.size(); a++) {
        if (a + 1 < c.computer_sequence.size())
          next = a + 1;
        new_val = pow(this->_computers.at(c.computer_sequence[next].first)
                          ._markets[c.computer_sequence[next].second]
                          ._events_avg,
                      2);
        old_val = pow(this->_computers.at(c.computer_sequence[a].first)
                          ._markets[c.computer_sequence[a].second]
                          ._events_avg,
                      2);
        sc = _computers.at(c.computer_sequence[a].first)._events_sum_square +
             (new_val - old_val);
        sum_changed += sc;
        sum_remv +=
            _computers.at(c.computer_sequence[a].first)._events_sum_square;
      }
      sumx2 = this->_variance + (this->_computers.size() * (pow(_mean, 2)));
      new_sum = sumx2 - sum_remv + sum_changed;
      new_var = new_sum - (this->_computers.size() * (pow(this->_mean, 2)));
      return new_var;
    }
    move_val = pow(this->_computers.at(c.from)._markets[c.who]._events_avg, 2);
    remove_comp = this->_computers.at(c.from)._events_sum_square - move_val;
    add_comp = this->_computers.at(c.to)._events_sum_square + move_val;
    sumx2 = this->_variance + (this->_computers.size() * (pow(this->_mean, 2)));
    new_sum = sumx2 - remove_comp + add_comp;
    new_var = new_sum - (this->_computers.size() * (pow(this->_mean, 2)));
    return new_var;
  };

  inline bool check_validity(std::vector<ul> &hard_constraints) const {
    for (std::pair<ul, computer> i : this->_computers) {
      if (!i.second.check_computer(hard_constraints)) {
        printf("Invalid: computer %lu has exceeded the limit.\n", i.first);
        return false;
      }
    }
    return true;
  };

  std::unordered_map<ul, computer> _computers;
  ul _events_sum;
  ld _mean, _variance, _std_deviation, _value;
};

#endif // SOLUTION_H