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

  inline void print() const {
    for (const auto &i : this->_computers)
      i.second.print();
    printf("Number of computers: %lu\nMean: %.5Lf\nVariance: %.5Lf\nStandard "
           "Deviation: %.5Lf\nSolution Value: %.5Lf\n",
           this->_computers.size(), this->_mean, this->_variance,
           this->_std_deviation, this->_value);
  };

  inline ul markets_in_solution() const {
    ul sum = 0;
    for (const auto &i : this->_computers)
      sum += i.second._markets.size();
    return sum;
  };

  inline void apply_change(const change c, const ld new_value){};

  inline void calc_mean() {
    ld sum = 0.0;
    for (const auto &i : this->_computers)
      sum += i.second._events_sum;
    this->_mean = sum / this->_computers.size();
  };

  inline void calc_variance() {
    ld sum = 0.0;
    for (const auto &i : this->_computers)
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
    this->calc_mean(), this->calc_variance(), this->calc_std_deviation(),
        this->_value = this->_std_deviation;
  };

  inline ld partial_eval(const change &c) const {
    /*if(c.change_type > 0){
        for()
    }*/
    return 0.0; // Placeholder for partial evaluation logic
  };

  /*
Variancia: sum_x2_new - n * (mean_new)^2
Somatorio var + n * (mean_new)^2

  # Entradas:
# sum_x: soma dos n elementos atuais
# sum_x2: soma dos quadrados dos n elementos atuais
# n: número total de elementos
# removed: lista com os k elementos que serão removidos
# added: lista com os k novos elementos que serão adicionados

# Passo 1: Atualizar soma e soma dos quadrados
sum_removed = SUM(removed)
sum_removed2 = SUM_SQUARES(removed)

sum_added = SUM(added)
sum_added2 = SUM_SQUARES(added)

# Passo 2: Atualizar valores agregados
sum_x_new = sum_x - sum_removed + sum_added
sum_x2_new = sum_x2 - sum_removed2 + sum_added2

# Passo 3: Calcular nova média
mean_new = sum_x_new / n

# Passo 4: Calcular novo somatório da variância (total, não dividida por n)
var_sum_new = sum_x2_new - n * (mean_new)^2

# Saída:
# var_sum_new: novo somatório da variância`
  */

  std::unordered_map<ul, computer> _computers;
  ul _events_sum, _events_sum_squared;
  ld _mean, _variance, _std_deviation, _value;
};

#endif // SOLUTION_H