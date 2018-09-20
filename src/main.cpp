#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>

#include "dfa.h"

using std::uint32_t;

int main(int argc, char* argv[]) {
    //TODO parameters parsing
     
    if (argc != 3) {
        std::cerr << "Wrong parameters." 
                  << " Usage \"dfa_minimization input_file output_file\"";
    }

    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);

    DFA dfa;

    std::string start_state_name;
    input >> start_state_name;
    dfa.add_state(start_state_name, false, true);

    uint32_t number_of_accept_state;
    input >> number_of_accept_state;

    while (number_of_accept_state--) {
        std::string state_name;
        input >> state_name;
        dfa.add_state(state_name, true, false);
    }

    uint32_t number_of_edges;
    input >> number_of_edges;

    while (number_of_edges--) {
        std::string from, to, symbol;
        input >> from >> to >> symbol;
        dfa.add_edge(from, to, symbol);
    }
    
    dfa.minimize();
    dfa.draw_in_DOT(output);

    return 0;
}
