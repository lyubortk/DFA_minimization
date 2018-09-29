#include <istream>
#include <ostream>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <unistd.h>
#include "dfa.h"

using std::uint16_t;

void process_dfa(std::istream& input, std::ostream& output, bool minimize) {
    DFA dfa;

    std::string start_state_name;
    input >> start_state_name;
    dfa.add_state(start_state_name, false, true);

    uint16_t number_of_accept_states;
    input >> number_of_accept_states;

    while (number_of_accept_states--) {
        std::string state_name;
        input >> state_name;
        dfa.add_state(state_name, true, false);
    }

    std::uint32_t number_of_edges;
    input >> number_of_edges;

    while (number_of_edges--) {
        std::string from, to, symbol;
        input >> from >> to >> symbol;
        dfa.add_edge(from, to, symbol);
    }
    
    if (minimize) {
        dfa.minimize();
    }

    dfa.draw_in_DOT(output);
}

void print_usage(char* name) {
    std::cerr << "USAGE: " << name << " [options] input_file\n"
          << "OPTIONS:\n"
          << "-n                 output a graph without minimazing the DFA\n"
          << "-o <file_name>     specify output file name "
          << "(default is \"<input_file>.out\")\n";
}

int main(int argc, char* argv[]) {
    std::string output_name;
    bool minimize = true; 
    
    char opt;
    while ((opt = getopt(argc, argv, "no:")) != -1) {
        if (opt == 'n') {
            minimize = false;
        } else if (opt == 'o') {
            output_name = std::string(optarg);
        } else {
            std::cerr << "unknown argument: " << opt << '\n';            
            print_usage(argv[0]);
            return 0;
        }
    }  
    
    if (optind + 1 != argc) {
        std::cerr << "wrong number of arguments\n";
        print_usage(argv[0]);
        return 0;
    }
    
    std::ifstream input(argv[optind]);
    std::ofstream output;
    if (output_name != "") {
        output.open(output_name);
    } else {
        output.open(std::string(argv[optind]) + ".out");
    }
    
    process_dfa(input, output, minimize);

    return 0;
}
