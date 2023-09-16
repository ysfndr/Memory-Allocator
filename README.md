# Memory-Allocator
A basic memory allocator program in C that implements a linked list data structure to manage memory allocation and deallocation. It uses four allocation algorithms: first fit, best fit, worst fit, and next fit.
This program allocates a large pool of memory (arena) on startup and, uses it to manage its own memory without having to trigger system calls. The program utilizes four types of memory management: First Fit, Best Fit, Worst Fit, and Next Fit.
The benchmarks are designed to test the speeds of the 4 fit algorithms as well as the case of using malloc instead of using arena.
