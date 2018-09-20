# DFA_minimization
DFA minimization utility for linux. Utility minimizes a DFA given in a txt file and outputs it in DOT language.

## Prerequisites
* GCC
* GNU Make
* Graphviz (optional)

## Input file format
```
name of the start state             //a string without whitespaces
number of accept states             //non-negative integer less than 65,536
names of accept states separated by whitespace
number of edges between states      // non-negative 32-bit integer
edge_1                              // edges are given by 3 strings without whitespaces:
....                                // 1. name of "from" state  2. name of "to" state 
edge_N                              // 3.symbol of alphabet
```
Example:
```
S
3
S A B
6
S A a
A B a
B S a
S B b
B A b
A S b
```


## Building and usage

1. Building an executable ("dfa_minimizer"):
```
make
```

2. Minimizing and transforming a DFA to DOT language:
```
dfa_minimizer input_file output_file
```

3. Transforming a DFA to DOT language without minimizing:
```
dfa_minimizer input_file output_file d
```

## Minimizing a DFA and visualizing it (requires Grapviz)
An executable of the minization utility should be already built.

1. Minimizing and visualizing a DFA:
``` 
vizualizer.sh input_file output_file.png
```

2. Visualizing a DFA without visualization:
```
vizualizer.sh input_file output_file.png d
```
