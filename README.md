# fast-validating-io

A fast input reading and validating template, primarily developed for use with the [DMOJ](https://dmoj.ca) ([github](https://github.com/DMOJ/judge-server)).

`Input.h` can be included in the program configuration directory to allow checker and interactor programs to have access to the input reading class
which reads input from the user or the judge while ensuring it is in the right format.

The main appeal of using this over other existing input validation templates is performance.
A good amount of care was put into making sure all of the functions are efficient and execute quickly.
Characters are read and stored into a buffer and parsed manually, providing a significant performance increase for
problems with a large amount of input or output.

C++ version at least 20 is required.
