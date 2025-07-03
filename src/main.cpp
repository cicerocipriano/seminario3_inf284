#include "Genetic.h"
#include "argh.h"

using ul = unsigned long;
using ld = long double;
using namespace std;

inline void usage() {
  printf("  -h    help\n"
         "  -t    type of heuristic (0 -> random, 1 -> greedy, 2 "
         "-> IG, 3 -> GA)\n  -n    number of computers\n  -f    file name, "
         "without extension (ex: 2e2c)\n\n   Example: ./sem -t 2 -n 2 -f "
         "2e2c\n\n");
  exit(EXIT_SUCCESS);
}

inline void random_solution(instance inst) {
  Genetic g = Genetic(inst);
  solution sol;
  do {
    sol = g.gen_random_sol(inst);
  } while (!sol.check_validity(inst._hard_constraints));
  sol.evaluation(), sol.calc_soft_value(inst._hard_constraints);
  sol.print(false);
}

inline void greedy_solution(instance inst) {
  solution sol;
  initial_greedy(inst, sol);
  sol.evaluation(), sol.calc_soft_value(inst._hard_constraints);
  sol.print(false);
}

inline void ig(instance inst) {
  solution sol;
  iterated_greedy(inst, sol);
}

inline void ga(instance inst) {
  Genetic g = Genetic(inst);
  g.solve(inst);
}

int main(int argc, char **argv) {
  ul type, nc;
  string file;
  argh::parser cmdl;
  cmdl.parse(argc, argv);
  if (cmdl[{"-h"}])
    usage();
  cmdl("-t", 1) >> type, cmdl("-n", 1) >> nc, cmdl("-f", 1) >> file;

  instance inst(nc);
  inst.read_input(file);

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
