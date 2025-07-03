#include "computer.h"
#include "instance.h"
#include "solution.h"
#include <list>
#include <queue>
#include <random>
#include <unordered_set>

using ul = unsigned long;
using ld = long double;

constexpr ul ITERATIONS_LOCAL_SEARCH = 100000;
constexpr ul ITERATIONS_ITERATED_GREEDY = 1000;
constexpr ul ITERATIONS = 100;

std::mt19937_64 eng{std::random_device{}()};
std::uniform_int_distribution<ul> dist{0, std::numeric_limits<ul>::max()};

inline change gen_change(solution &sol) {
  change c;
  c.change_type = dist(eng) % sol._computers.size();
  if (c.change_type == 0) {
    c.from = dist(eng) % sol._computers.size();
    c.who = dist(eng) % sol._computers[c.from]._markets.size();
    while (true) {
      c.to = dist(eng) % sol._computers.size();
      if (c.to != c.from)
        break;
    }
  } else {
    ul aux, num = 0;
    std::unordered_set<ul> used;
    while (true) {
      aux = dist(eng) % sol._computers.size();
      if (used.find(aux) == used.end()) {
        used.insert(aux), num++;
        if (num == c.change_type + 1)
          break;
      }
    }
    while (!used.empty()) {
      c.computer_sequence.push_back({*used.begin(), 0});
      used.erase(used.begin());
    }
    for (std::pair<ul, ul> &i : c.computer_sequence)
      i.second = dist(eng) % sol._computers.at(i.first)._markets.size();
  }
  return c;
};

inline void initial_greedy(instance &inst, solution &sol) {
  std::sort(inst._markets.begin(), inst._markets.end(),
            [](const market &a, const market &b) {
              return a._events_avg > b._events_avg;
            });
  std::list<market> v(inst._markets.begin(), inst._markets.end());
  ul idx = 0;
  computer c(0);
  bool found = false;
  bool (*compare)(const computer &, const computer &) = [](const computer &a,
                                                           const computer &b) {
    return a._events_sum > b._events_sum;
  };
  std::priority_queue<computer, std::vector<computer>, decltype(compare)>
      min_heap(compare);
  for (ul i = 0; i < inst._k; ++i)
    min_heap.push({inst._n_exchanges});
  while (!v.empty()) {
    c = min_heap.top(), found = false, min_heap.pop();
    for (std::list<market>::iterator it = v.begin(); it != v.end(); ++it) {
      if (c._constraint_count[it->_exchange_id] <
          inst._hard_constraints[it->_exchange_id]) {
        c._markets.push_back(*it), c._events_sum += it->_events_avg;
        c._constraint_count[it->_exchange_id]++, v.erase(it), found = true;
        break;
      }
    }
    if (!found)
      printf("This solution is invalid.\n"), exit(EXIT_FAILURE);
    min_heap.push(c);
  }
  while (!min_heap.empty())
    sol._computers.insert({idx++, min_heap.top()}), min_heap.pop();
  sol.evaluation();
};

inline void reconstruct_greedy(solution &sol, instance &inst) {
  std::vector<market> markets;
  std::unordered_set<ul> affected_computers;
  ul num_k = dist(eng) % sol._computers.size() + 1, chosen_computer, start, end;
  std::pair<ul, computer> p;
  while (affected_computers.size() != num_k) {
    chosen_computer = dist(eng) % sol._computers.size();
    if (affected_computers.find(chosen_computer) == affected_computers.end())
      affected_computers.insert(chosen_computer);
  }
  for (ul i : affected_computers) {
    end = dist(eng) % sol._computers.at(i)._markets.size();
    if (end == 0)
      end = sol._computers.at(i)._markets.size() - 1;
    start = dist(eng) % (sol._computers.at(i)._markets.size() - end);
    for (std::vector<market>::iterator j =
             sol._computers.at(i)._markets.begin() + start;
         j != sol._computers.at(i)._markets.begin() + start + end; j++) {
      markets.push_back(*j);
      sol._computers.at(i)._events_sum -= j->_events_avg;
      sol._computers.at(i)._constraint_count[j->_exchange_id]--;
    }
    sol._computers.at(i)._markets.erase(
        sol._computers.at(i)._markets.begin() + start,
        sol._computers.at(i)._markets.begin() + start + end);
  }
  std::sort(markets.begin(), markets.end(),
            [](const market &a, const market &b) {
              return a._events_avg > b._events_avg;
            });
  std::list<market> markets_list(markets.begin(), markets.end());
  bool found = false;
  bool (*compare)(const std::pair<ul, computer> &,
                  const std::pair<ul, computer> &) =
      [](const std::pair<ul, computer> &a, const std::pair<ul, computer> &b) {
        return a.second._events_sum > b.second._events_sum;
      };
  std::priority_queue<std::pair<ul, computer>,
                      std::vector<std::pair<ul, computer>>, decltype(compare)>
      min_heap(compare);
  for (const std::pair<ul, computer> i : sol._computers)
    min_heap.push(i);
  while (!markets_list.empty()) {
    p = min_heap.top(), min_heap.pop(), found = false;
    for (std::list<market>::iterator it = markets_list.begin();
         it != markets_list.end(); ++it) {
      if (p.second._constraint_count[it->_exchange_id] <
          inst._hard_constraints[it->_exchange_id]) {
        p.second._markets.push_back(*it);
        p.second._events_sum += it->_events_avg;
        p.second._constraint_count[it->_exchange_id]++;
        markets_list.erase(it), found = true;
        break;
      }
    }
    if (!found)
      printf("This solution is invalid.\n"), exit(EXIT_FAILURE);
    min_heap.push(p);
  }
  while (!min_heap.empty())
    p = min_heap.top(), min_heap.pop(), sol._computers.at(p.first) = p.second;
  sol.evaluation();
};

inline void local_search(solution &sol) {
  change c;
  ld new_value;
  for (ul i = 0; i < ITERATIONS_LOCAL_SEARCH; ++i) {
    c = gen_change(sol), new_value = sol.partial_eval(c);
    if (new_value < sol._value) {
      sol.apply_change(c,
                       new_value) /*, printf("LS found a better solution\n")*/;
      return;
    }
  }
  // printf("No better solution found in the LS.\n");
};

inline void iterated_greedy(instance &inst, solution &sol) {
  bool (*compare)(const solution &, const solution &) = [](const solution &a,
                                                           const solution &b) {
    if (std::round(a._value * 10000.0L) / 10000.0L >
        std::round(b._value * 10000.0L) / 10000.0L)
      return true;
    else if (std::round(a._value * 10000.0L) / 10000.0L <
             std::round(b._value * 10000.0L) / 10000.0L)
      return false;
    else
      return a._soft_value < b._soft_value;
  };
  std::priority_queue<solution, std::vector<solution>, decltype(compare)>
      solutions(compare);
  ul /*aux = 0, */ best_value = std::numeric_limits<ul>::max(), idx = 0;
  initial_greedy(inst, sol);
  for (ul j = 0; j < ITERATIONS; j++) {
    do {
      for (ul i = 0; i < ITERATIONS_ITERATED_GREEDY; ++i)
        local_search(sol), reconstruct_greedy(sol, inst);
      local_search(sol) /*, printf("Iteration %lu:\n", aux++)*/;
    } while (!sol.check_validity(inst._hard_constraints));
    sol.calc_soft_value(inst._hard_constraints);
    solutions.push(sol);
  }

  printf("Final solution:\n");
  solutions.top().print(false);
};