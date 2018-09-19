#include <unordered_map>
#include <string>

#include "dfa.h"

using std::uint32_t;

DFA::DFA() : number_of_states(0) {
}

void DFA::add_state(const std::string& state_name, bool accept_state) {
    if (state_name_to_int.count(state_name) == 0) {
        state_name_to_int[state_name] = number_of_states++;
        int_to_state_name.push_back(state_name);
        is_accept_state.push_back(accept_state);
        graph.push_back({});
    }
}

void DFA::add_edge(const std::string& from, const std::string& to, 
                                       const std::string& symbol) {
    add_state(from, false);
    add_state(to, false);
    uint32_t from_int = state_name_to_int[from];
    uint32_t to_int = state_name_to_int[to];
    graph[from_int].push_back({to_int, symbol});
}

void DFA::minimize() {
    
}

void DFA::draw_in_DOT(std::ostream& out) {
    const std::string indent = "    ";
    out << "digraph gr{\n"
        << indent << "rankdir=LR;\n"
        << indent 
        << "node [shape = doublecircle, color=black, fontcolor=black];\n";

    for (uint32_t i = 0; i < number_of_states; ++i) {
        if (is_accept_state[i]) {
            out << indent << int_to_state_name[i] << '\n';    
        }
    }
    
    out << indent << "node [shape = circle];\n";
    
    for (uint32_t i = 0; i < number_of_states; ++i) {
        std::unordered_map<uint32_t, std::string> merged_edges;

        for (Edge& e: graph[i]) {
            if (merged_edges[e.destination].size() > 0) {
                merged_edges[e.destination].append(", " + e.symbol);
            } else {
                merged_edges[e.destination].append(e.symbol);
            }
        }

        for (std::pair<uint32_t, std::string> e: merged_edges) {
            //TODO regex_replace
            out << indent << int_to_state_name[i] << " -> "
                << int_to_state_name[e.first] << " [label = \""
                << e.second << "\"]\n";
        }
    }

    out << "}";
}

