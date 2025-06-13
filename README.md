# seminario3_inf284
This repository contains the codebase for a metaheuristics project aimed at solving the market partitioning problem in machines. It explores optimization techniques to effectively divide markets among available machines.

```mermaid
  flowchart TB
    phase1[Python script to generate instances as JSONs, it won't be on this repository.]
    phase2[Create the abstractions for better data visualization: class Instance, Solution, etc.]
    phase3[JSON reader using the Nlohmann header.]
    phase4[Write the greedy and the known optimum algorithm for solving the problem.]
    phase5[Neighbor generation methods.]
    phase6[Create the soft constraint's effect on the solution.]
    phase7[Write the Local Search algorithm.]
    phase8[Write the Iterated Greedy algorithm.]
    phase9[Write the Genetic Algorithm.]
    phase10[Testing]
    phase1 --- phase2
    phase3 --- phase4
    phase3 --- phase5
    phase3 --- phase6
    phase4 --- phase5
    phase4 --- phase6
    phase5 --- phase6
    phase8 --- phase9
    phase1 --> phase3
    phase2 --> phase3
    phase2 --> phase4
    phase2 --> phase5
    phase2 --> phase6
    phase5 --> phase7
    phase6 --> phase7
    phase4 --> phase8
    phase7 --> phase8
    phase5 --> phase9
    phase3 --> phase10
    phase8 --> phase10
    phase9 --> phase10
```
