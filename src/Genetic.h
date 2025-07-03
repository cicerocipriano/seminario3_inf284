#ifndef GENETIC_H
#define GENETIC_H

#include "algorithms.h"

using ul = unsigned long;
using ld = long double;

struct std_comp {
  bool operator()(const solution &a, const solution &b) const {
    return a._std_deviation > b._std_deviation;
  }
};

struct Genetic {
  std::priority_queue<solution, std::vector<solution>, std_comp> sols;
  std::random_device rd;
  std::mt19937 eng;
  std::uniform_int_distribution<> dist, dist2;
  ul pop_size, iterations;
  std::vector<ul> hard_c;

  inline Genetic(instance i) {
    this->pop_size = 5000, this->iterations = 1000,
    this->sols =
        std::priority_queue<solution, std::vector<solution>, std_comp>();
    this->hard_c = std::vector<ul>(i._n_exchanges);
    for (std::pair<ul, ul> i : i._hard_constraints)
      this->hard_c[i.first] = i.second;

    this->eng = std::mt19937(this->rd()),
    this->dist = std::uniform_int_distribution<>(0, 1),
    this->dist2 = std::uniform_int_distribution<>(0, 1);
    ul a = 0;
    while (a < this->pop_size / 2) {
      a++;
      solution s = this->gen_random_sol(i);
      if (!s.check_validity(i._hard_constraints)) {
        a--;
        continue;
      }
      s.evaluation();
      this->sols.emplace(s);
    }
    while (a < this->pop_size / 2) {
      solution s = solution();
      initial_greedy(i, s);
      this->sols.emplace(s);
    }
  }

  inline solution gen_random_sol(instance i) {
    std::vector<market> mks = i._markets;
    solution s = solution();
    for (ul a = 0; a < i._k; a++) {
      computer c = computer();
      s._computers.insert({a, c});
    }
    this->dist = std::uniform_int_distribution<>(0, s._computers.size() - 1);
    for (ul a = 0; a < i._markets.size(); a++) {
      market m = i._markets[a];
      ul inx = this->dist(this->eng);
      s._computers.at(inx)._markets.push_back(m);
    }
    for (ul a = 0; a < i._k; a++)
      s._computers.at(a).calc_sum();
    return s;
  }

  inline void make_children(
      instance i, solution p1, solution p2,
      std::priority_queue<solution, std::vector<solution>, std_comp> &hold) {
    try {
      solution a1b1 = p1, b1a1 = p2, b1a2 = p2, a1b2 = p1;
      solution a2b2 = solution(), b2a2 = solution();
      ul half = p1._computers.size() / 2;
      if (p1._computers.size() != p2._computers.size())
        printf("%lu Ack %lu\n", p1._computers.size(), p2._computers.size());
      for (ul a = 0; a < 5; a++) {
        a1b1._computers.at(half + a) = p2._computers.at(a);
        b1a1._computers.at(half + a) = p1._computers.at(a);
        b1a2._computers.at(half + a) = p1._computers.at(a + half);
        a1b2._computers.at(half + a) = p2._computers.at(a + half);
        a2b2._computers.insert({a, p1._computers.at(a + 0 + half)});
        b2a2._computers.insert({a, p2._computers.at(a + half)});
      }
      for (ul a = 0; a < 5; a++) {
        a2b2._computers.insert({a + half, p2._computers.at(a + half)});
        b2a2._computers.insert({a + half, p1._computers.at(a + half)});
      }
      a1b1.evaluation(), a1b2.evaluation();
      b1a2.evaluation(), a2b2.evaluation();
      if (a1b1.check_validity(i._hard_constraints))
        hold.emplace(a1b1);
      if (a1b2.check_validity(i._hard_constraints))
        hold.emplace(a1b2);
      if (b1a2.check_validity(i._hard_constraints))
        hold.emplace(b1a2);
      if (a2b2.check_validity(i._hard_constraints))
        hold.emplace(a2b2);
    } catch (int errorCode) {
      printf("Error in make_children: %d\n", errorCode);
    }
  }

  inline void next_generation(instance i) {
    std::vector<solution> good_sols = std::vector<solution>();
    std::vector<solution> elite_sols = std::vector<solution>();
    ul good_limit = this->sols.size() / 4;
    ul elite_limit = (this->sols.size() * 3) / 100;
    for (ul a = 0; a < good_limit; a++) {
      solution s = this->sols.top();
      this->sols.pop();
      if (a < elite_limit)
        elite_sols.push_back(s);
      good_sols.push_back(s);
    }
    this->sols =
        std::priority_queue<solution, std::vector<solution>, std_comp>();
    std::ranges::shuffle(good_sols.begin(), good_sols.end(), eng);
    for (ul a = 0; a < 25; a++)
      solution s = good_sols[a];
    for (ul a = 0; a < 25; a++) {
      ul next = 0;
      if (a + 1 < 25)
        next = a + 1;
      this->make_children(i, good_sols[a], good_sols[next], this->sols);
    }
    while (this->sols.size() < good_limit || this->sols.size() % 2 != 0) {
      solution s = gen_random_sol(i);
      s.evaluation(), this->sols.emplace(s);
    }
    for (int a = 0; a < elite_sols.size(); a++)
      this->sols.emplace(elite_sols[a]);
  }

  inline void solve(instance i) {
    bool (*compare)(const solution &, const solution &) =
        [](const solution &a, const solution &b) {
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
        best_it_sols(compare);
    for (int a = 0; a < iterations; a++) {
      this->next_generation(i);
      best_it_sols.push(this->sols.top());
    }
    best_it_sols.top().print(false);
  }
};

#endif