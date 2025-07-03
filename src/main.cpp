#include "Genetic.h"

using ul = unsigned long;
using ld = long double;
using namespace std;

inline void usage() {
  printf("\n\tSeminario 4\n\n\t-h\t\thelp\n"
         "\t-t\t\ttype of heuristic (0 -> random, 1 -> greedy, 2 "
         "-> IG, 3 -> GA)\n\t-n\t\tnumber of computers\n\t-f\t\tfile name, "
         "without extension (ex: 2e2c)\n\n\tExample: ./sem -t 2 -n 2 -f "
         "2e2c\n\n");
  exit(EXIT_SUCCESS);
}

inline void random_solution(instance inst) {
  Genetic g = Genetic(inst);
  solution sol;
  do {
    sol = g.gen_random_sol(inst);
  } while (!sol.check_validity(inst._hard_constraints));
  sol.evaluation();
  sol.calc_soft_value(inst._hard_constraints);
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
