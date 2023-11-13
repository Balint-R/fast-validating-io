# fast-validating-io

A fast input reading and validating template, primarily developed for use with the [DMOJ](https://dmoj.ca) ([github](https://github.com/DMOJ/judge-server)).

`Input.h` can be included as a separate file in the program configuration directory to allow checker and interactor programs to access it.

The main appeal of using this over other existing input validation templates is performance.
All functions were written with execution speed in mind.
Characters are read and stored into a buffer and parsed manually, providing a significant performance increase for
problems with a large amount of input or output over using built-in methods.

C++ version at least 20 is required.

## Benchmarks

Time taken to read $10^7$ unsigned integers below the given bounds.
All benchmarks were done on the same machine running g++ 13 and `-O2` optimization level.

| Method | $a_i \le 10^3$ | $a_i \le 10^6$ | $a_i \le 10^9$ |
| - | - | - | - |
| `fread` | $70$ ms | $110$ ms | $155$ ms |
| `cin` with `cin.sync_with_stdio(0); cin.tie(0);` | $420$ ms | $530$ ms | $630$ ms |
| `scanf("%u")` | $670$ ms | $730$ ms | $880$ ms |
| Balint's `IdenticalReader` | $70$ ms | $120$ ms | $165$ ms |
| Balint's `StandardReader` | $85$ ms | $130$ ms | $180$ ms |
| Balint's `IdenticalInteractor` | $235$ ms | $390$ ms | $550$ ms |
| [riolku](https://github.com/Riolku/setter-suite)'s `IdenticalCheckerReader` | $540$ ms | $860$ ms | $1285$ ms |
| [wesley-a-leung](https://github.com/wesley-a-leung/Problem-Setting-Resources)'s `IdenticalReader` with `cin.sync_with_stdio(0); cin.tie(0);` | $550$ ms | $860$ ms | $1300$ ms |

