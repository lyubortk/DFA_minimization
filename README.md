# DFA_minimization
DFA minimization utility for linux. Utility minimizes a DFA given in a txt file and outputs the corresponding graph in DOT language.

## Prerequisites
* GCC
* GNU Make
* Graphviz (optional)

## Input file format
```
name of the start state             // a string without whitespaces
number of accept states             // non-negative integer less than 65,536
names of accept states              // strings separated by whitespace
number of edges between states      // non-negative 32-bit integer
edge_1                              // edges are given by 3 strings without whitespaces:
....                                // 1. name of "from" state  2. name of "to" state 
edge_N                              // 3. symbol of alphabet
```
    
**The number of edges has to be equal to (number of different state names) * (number of elements in the alphabet).**

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


## Building

Building an executable ("dfa_minimizer"):
```
make
```

## Minimizing a DFA and transforming it to DOT language
```
dfa_minimizer [OPTIONS] input_file
```
OPTIONS:  
```
-n              output the graph without minimizing the DFA  
-o <file_name>  specify output file name (default one is <input_file>.out)
```

## Minimizing a DFA and visualizing it (requires Graphviz)
An executable of the minization utility ("dfa_minimizer") should be already built.

``` 
visualizer.sh [OPTIONS] input_file
```
OPTIONS:
```
-n              visualize the graph without minimizing the DFA  
-o <file_name>  specify output file name (default one is <input_file>.png)
```
