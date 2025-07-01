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

  inline void apply_change(const change c, const ld new_value)
  {

      _variance = new_value;
      _std_deviation = sqrt(_variance);
      _value = _std_deviation;
      if (c.change_type > 0)
      {
          auto firstx = c.computer_sequence[0];
          ul prev;
          ul after;
          ul first_val = _computers.at(firstx.first)._markets[firstx.second]._events_avg;
          for (int a = 1; a < c.computer_sequence.size(); a++)
          {
              prev = _computers.at(c.computer_sequence[a].first)._markets[c.computer_sequence[a].second]._events_avg;
              std::swap(_computers.at(firstx.first)._markets[firstx.second], _computers.at(c.computer_sequence[a].first)._markets[c.computer_sequence[a].second]);
              after = _computers.at(c.computer_sequence[a].first)._markets[c.computer_sequence[a].second]._events_avg;
              _computers.at(c.computer_sequence[a].first)._events_sum += after - prev;



          }

          _computers.at(firstx.first)._events_sum += _computers.at(firstx.first)._markets[firstx.second]._events_avg - first_val;



      }
      else 
      {
              // ul remove_comp = _computers.at(c.from)._events_sum_square -  move_val;
          auto last = _computers.at(c.from)._markets.back();
          std::swap(_computers.at(c.from)._markets[c.who], last);
          auto wanted = _computers.at(c.from)._markets.back();
          _computers.at(c.to)._markets.push_back(wanted);
          _computers.at(c.from)._markets.pop_back();
          _computers.at(c.from)._events_sum -= wanted._events_avg;
          _computers.at(c.to)._events_sum += wanted._events_avg;

      }
  };

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
    if (c.change_type > 0)
    {

          ul sum_changed = 0;
          ul sum_remv = 0;
          for (int a = 0; a < c.computer_sequence.size(); a++)
          {

              int next = 0;
              if (a + 1 < c.computer_sequence.size()) { next = a + 1; }
              ul new_val = _computers.at(c.computer_sequence[next].first)._markets[c.computer_sequence[next].second]._events_avg;
              ul old_val = _computers.at(c.computer_sequence[a].first)._markets[c.computer_sequence[a].second]._events_avg;
              new_val = pow(new_val,2);
              old_val = pow(old_val, 2);

              ul sc = _computers.at(c.computer_sequence[a].first)._events_sum_square + (new_val - old_val);
              sum_changed += sc;
              sum_remv += _computers.at(c.computer_sequence[a].first)._events_sum_square;


          }

          ul sumx2 = _variance + (_computers.size() * (pow(_mean,2)));
          ul new_sum = sumx2 - sum_remv + sum_changed;
          ul new_val = new_sum - (_computers.size() * (pow(_mean, 2)));


          return new_val;




    }

    else 
    {
        



        ul move_val = _computers.at(c.from)._markets[c.who]._events_avg;
        move_val = pow(move_val, 2);
        
        ul remove_comp = _computers.at(c.from)._events_sum_square -  move_val;
        ul add_comp = _computers.at(c.to)._events_sum_square + move_val;
     

        ul sumx2 = _variance + (_computers.size() * (pow(_mean, 2)));
        ul new_sum = sumx2 - remove_comp + add_comp;
        ul new_val = new_sum - (_computers.size() * (pow(_mean, 2)));

        return new_val;



    }

    
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
  ul _events_sum;
  ld _mean, _variance, _std_deviation, _value;
};

#endif // SOLUTION_H