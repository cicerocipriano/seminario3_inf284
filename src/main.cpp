#include "Genetic.h"
#include <iostream>

using ul = unsigned long;
using ld = long double;
using namespace std;

int main(int argc, char **argv) {

  
  instance inst(stoul(argv[argc - 2])), aux = inst;
  inst.read_input(argv[argc - 1]);

  
  Genetic g = Genetic(inst);
  g.solve(inst);

  

  cout << "Genetico" << endl;
  solution sol;
  iterated_greedy(inst, sol);
  return 0;

}
