# THE-Haskell-Compiler

CSC488 Compiler Project for Winter 2021. 

Compiles a (small) subset of Haskell into standard C. Driven by Python Lex-Yacc (PLY).
Intermediate code representation is C code, which gets directly compiled.

Supports core Haskell language features such as currying, lazy evaluation, continuation passing style, infinite lists, by implementating "Cheney On The MTA" garbage collection routines so that we don't run out of stack space. 
