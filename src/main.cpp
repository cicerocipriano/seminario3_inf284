#include "Genetic.h"

using ul = unsigned long;
using ld = long double;
using namespace std;

inline void usage() {
  printf("\n\tSeminario 4\n\n\t-h\t\thelp\n"
         "\t-t\t\ttype of heuristic (0 -> random, 1 -> greedy, 2 "
         "-> IG, 3 -> GA)\n\t-n\t\tnumber of computers\n\t-f\t\tfile name, "
         "without extension (ex: 2e2c)\n\n\tExample: ./sem -t 2 -n 2 -f "
         "2e3c\n\n");
  exit(EXIT_SUCCESS);
}

inline void last_check(instance inst, solution sol) {
  printf("\n\n\nLast Checking of the solution.\n\n");
  if (sol._computers.size() != inst._k) {
    printf("Wrong number of computers\n");
    return;
  }
  if (inst._n_markets != inst._markets.size()) {
    printf("Problem in the reading, number of markets not matching int the "
           "instance object.\n");
    return;
  }
  vector<ul> markets(inst._markets.size(), 0);
  for (pair<ul, computer> p : sol._computers) {
    for (market m : p.second._markets)
      markets[m._market_id]++;
  }
  for (auto m : markets) {
    if (m != 1) {
      printf(
          "The solution is invalid, there are missing or repeated markets\n");
      return;
    }
  }
  ul total_sum = 0, aux_sum = 0;
  for (auto m : inst._markets)
    total_sum += m._events_avg;
  for (auto i : sol._computers) {
    aux_sum += i.second.calc_sum();
  }
  if (total_sum != aux_sum) {
    printf("Total events are different from the instance's\n");
    return;
  }
  for (auto i : sol._computers) {
    vector<ul> constraint_count(inst._hard_constraints.size(), 0);
    for (auto m : i.second._markets)
      constraint_count[m._exchange_id]++;
    for (ul j = 0; j < constraint_count.size(); j++) {
      if (constraint_count[j] > inst._hard_constraints.at(j)) {
        printf("Constraint exceeded in the solution\n");
        return;
      }
    }
    constraint_count.clear();
  }
  sol.evaluation();
  sol.calc_soft_value(inst._hard_constraints);
  sol.print(true);
  printf("Viable solution confirmed\n");
}

inline void random_solution(instance inst) {
  instance aux = inst;
  Genetic g = Genetic(inst);
  solution sol;
  do {
    sol = g.gen_random_sol(inst);
  } while (!sol.check_validity(inst._hard_constraints));
  last_check(aux, sol);
}

inline void greedy_solution(instance inst) {
  instance aux = inst;
  solution sol;
  initial_greedy(inst, sol);
  last_check(aux, sol);
}

inline void ig(instance inst) {
  instance aux = inst;
  solution sol;
  iterated_greedy(inst, sol);
  last_check(aux, sol);
}

inline void ga(instance inst) {
  instance aux = inst;
  Genetic g = Genetic(inst);
  solution s = g.solve(inst);
}

int main(int argc, char **argv) {
  ul type = 0, nc = 0;
  string file;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-h") == 0)
      usage();
    else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc)
      type = std::stoul(argv[++i]);
    else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc)
      nc = std::stoul(argv[++i]);
    else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc)
      file = argv[++i];
  }

  instance inst(nc);
  inst.read_input(file);
  inst.print();

  switch (type) {
  case 1:
    greedy_solution(inst);
    break;

  case 2:
    ig(inst);
    break;

  case 3:
    ga(inst);
    break;

  default:
    random_solution(inst);
    break;
  }

  return 0;
}
