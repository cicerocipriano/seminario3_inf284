# seminario3_inf284
This repository contains the codebase for a metaheuristics project aimed at solving the market partitioning problem in machines. It explores optimization techniques to effectively divide markets among available machines.

```mermaid
  flowchart TB
    phase1{{Script to generate JSON instances, won't be on the repo.}}
    phase2{{Abstractions for better visualization.}}
    phase3{{JSON reader.}}
    phase4{{Greedy and the known optimum algorithms.}}
    phase5{{Neighbor generation methods.}}
    phase6{{Soft constraint's effect on the solution.}}
    phase7{{Local Search algorithm.}}
    phase8{{Iterated Greedy algorithm.}}
    phase9{{Genetic Algorithm.}}
    phase10{Testing.}
    phase1 --> phase3
    phase2 --> phase3
    phase2 --> phase4
    phase2 --> phase6
    phase2 --> phase5
    phase5 --> phase7
    phase6 --> phase7
    phase4 --> phase8
    phase7 --> phase8
    phase5 --> phase9
    phase3 --> phase10
    phase8 --> phase10
    phase9 --> phase10
```
