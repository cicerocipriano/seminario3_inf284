#include "algorithms.h"

using ul = unsigned long;
using ld = long double;
using namespace std;

int main(int argc, char **argv) {
    instance inst(stoul(argv[argc - 2])), aux = inst;
    inst.read_input(argv[argc - 1]);
    solution sol;
    initial_greedy(inst, sol);
    return 0;
}
