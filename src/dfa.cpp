#include <unordered_map>
#include <string>
#include <algorithm>
#include <set>
#include <iostream>

#include "dfa.h"

using std::uint32_t;

DFA::DFA() : number_of_states(0), start_state_number(0) {
}

void DFA::add_state(const std::string& state_name, bool accept_state,
                                                   bool start_state) {
    uint32_t state_number = -1;
    if (state_name_to_int.count(state_name) == 0) {
        state_number = number_of_states++;

        state_name_to_int[state_name] = state_number;
        int_to_state_name.resize(number_of_states);
        int_to_state_name[state_number] = state_name;

        is_accept_state.resize(number_of_states);
        graph.resize(number_of_states);
    }
    
    state_number = state_name_to_int[state_name];
    if (start_state) {
        start_state_number = state_number;
    }
    is_accept_state[state_number] |= accept_state;
}

void DFA::add_edge(const std::string& from, const std::string& to, 
                                       const std::string& symbol) {
    add_state(from, false, false);
    add_state(to, false, false);
    uint32_t from_int = state_name_to_int[from];
    uint32_t to_int = state_name_to_int[to];
    graph[from_int].push_back({to_int, symbol});
}

void DFA::minimize() {
    remove_unreachable_states(); 
    unite_states();     
}

void DFA::draw_in_DOT(std::ostream& out) const {
    const std::string indent = "    ";
    out << "digraph gr{\n"
        << indent << "rankdir=LR;\n"
        << indent << "node [shape = point, color=white, fontcolor=white]; "
        << "__start__;\n"
        << indent 
        << "node [shape = doublecircle, color=black, fontcolor=black];\n";

    for (uint32_t i = 0; i < number_of_states; ++i) {  // accept_states
        if (is_accept_state[i]) {
            out << indent << int_to_state_name[i] << ";\n";    
        }
    }
    
    out << indent << "node [shape = circle];\n";
    for (uint32_t i = 0; i < number_of_states; ++i) { // regular_states
        if (!is_accept_state[i]) {
            out << indent << int_to_state_name[i] << ";\n";    
        }
    }

    out << indent << "__start__ -> " << int_to_state_name[start_state_number]
        << '\n';

    for (uint32_t i = 0; i < number_of_states; ++i) { // drawing edges
        std::unordered_map<uint32_t, std::string> merged_edges;

        for (const Edge& e: graph[i]) {
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

bool DFA::Edge::operator<(const Edge& other) const {
    return symbol < other.symbol;
}

void DFA::dfs(uint32_t vert, std::vector<int>& used) {
    used[vert] = true;
    for (auto e: graph[vert]) {
        if (used[e.destination]) continue;
        dfs(e.destination, used);
    }
}

void DFA::remove_unreachable_states() {
    std::vector<int> used(number_of_states, 0);
    dfs(start_state_number, used);
    
    DFA replacement_dfa;
    replacement_dfa.add_state(int_to_state_name[start_state_number],
                              is_accept_state[start_state_number], true);

    for (uint32_t i = 0; i < number_of_states; i++) {
        if (used[i]) {
            replacement_dfa.add_state(int_to_state_name[i],
                                      is_accept_state[i], false);
            for (auto e: graph[i]) {
                replacement_dfa.add_edge(int_to_state_name[i],
                                         int_to_state_name[e.destination],
                                         e.symbol);
            }
        }
    }

    *this = replacement_dfa;
}

void DFA::dfs_on_pairs(uint32_t v1, uint32_t v2, 
                       std::vector<std::vector<char>>& used, 
                       std::vector<std::vector<std::vector<PairEdge>>>& gr) {
    used[v1][v2] = true; 
    for (auto e: gr[v1][v2]) {
        if (!used[e.destination_1][e.destination_2]) {
            dfs_on_pairs(e.destination_1, e.destination_2, used, gr);
        }
    }
}

void DFA::unite_states() {
    
    for (uint32_t i = 0; i < number_of_states; ++i) {
        std::sort(graph[i].begin(), graph[i].end());
    }
    
    std::vector<std::vector<std::vector<PairEdge>>> reversed_graph_on_pairs 
        (number_of_states, std::vector<std::vector<PairEdge>> 
                                                    (number_of_states));
    // NxNxM -> edge

    for (uint32_t i = 0; i < number_of_states; ++i) {
        for (uint32_t j = i+1; j < number_of_states; ++j) {

            uint32_t pointer_1 = 0, pointer_2 = 0;
            while (graph[i].size() > pointer_1 && 
                   graph[j].size() > pointer_2) {
                
                if (graph[i][pointer_1].symbol < graph[j][pointer_2].symbol) {
                    pointer_1++;
                } else if (graph[j][pointer_2].symbol < graph[i][pointer_1].symbol) {
                    pointer_2++;
                } else {
                    uint32_t dest_1 = graph[i][pointer_1].destination;
                    uint32_t dest_2 = graph[j][pointer_2].destination;
                    std::string symbol = graph[i][pointer_1]. symbol;

                    if (dest_1 > dest_2) { 
                        std::swap(dest_1, dest_2);
                    }
                    if (dest_1 != dest_2) {
                        reversed_graph_on_pairs[dest_1][dest_2].push_back(
                                                                {i, j, symbol});
                        //
                        //std::cout << dest_1 << ' ' << dest_2 << ' ' << i << ' ' << j << ' ' << symbol << '\n';
                    }
                    pointer_1++;
                    pointer_2++;
                }
            } // while sizes > pointers

        } //inner for loop
    } 

    std::vector<std::vector<char>> distinguishable (number_of_states,
                                     std::vector<char>(number_of_states, 0));
    
    for (uint32_t i = 0; i < number_of_states; ++i) {
        for (uint32_t j = i+1; j < number_of_states; ++j) {
            if (is_accept_state[i] != is_accept_state[j]) {
                dfs_on_pairs(i, j, distinguishable, reversed_graph_on_pairs);
            }
        }
    } 
    
   /* */
    for (int i = 0; i < (int)number_of_states; ++i) {
        for (int j = 0; j < (int)number_of_states; ++j) {
            std::cout << (bool)distinguishable[i][j] << ' ';
        }
        std::cout << '\n';
    }
  /* */


    DFA dfa_replacement;
      
    std::vector<uint32_t> head_element(number_of_states, UINT32_MAX);
    for (uint32_t i = 0; i < number_of_states; ++i) {
        if (head_element[i] != UINT32_MAX) continue;
        head_element[i] = i;
        std::string new_state_name = int_to_state_name[i];
        for (uint32_t j = i + 1; j < number_of_states; ++j) {
            if (!distinguishable[i][j]) {
                head_element[j] = i;
                new_state_name.append("," + int_to_state_name[j]);
            }
        }
        int_to_state_name[i] = new_state_name;
        dfa_replacement.add_state(new_state_name, is_accept_state[i], false);
    }
    
    uint32_t head_of_start_state = head_element[start_state_number];
    
    dfa_replacement.add_state(int_to_state_name[head_of_start_state],
                              is_accept_state[head_of_start_state], true);
    
    std::vector<std::set<Edge>> graph_on_heads(number_of_states);

    for (uint32_t i = 0; i < number_of_states; ++i) {
        for (auto e: graph[i]) {
            graph_on_heads[head_element[i]].insert({head_element[e.destination],e.symbol});        
        }
    }
    for (uint32_t i = 0; i < number_of_states; ++i) {
        if (head_element[i] != i) continue;
        for (auto e: graph_on_heads[i]) {
            dfa_replacement.add_edge(int_to_state_name[i], 
                                int_to_state_name[e.destination], e.symbol);
        }
    }

    *this = dfa_replacement;

}
