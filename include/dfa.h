#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <ostream>

class DFA {
public:
    DFA();
    ~DFA() = default;
    DFA(const DFA&) = default;
    DFA(DFA&&) = default;
    DFA& operator=(const DFA&) = default;

    void add_state(const std::string& state_name, bool accept_state);
    void add_edge(const std::string& from, const std::string& to, 
                                          const std::string& symbol);

    void minimize();
    void draw_in_DOT(std::ostream& out);
private:

    struct Edge {
        std::uint32_t destination;
        std::string symbol;
    };
    
    uint32_t number_of_states;
    std::unordered_map<std::string, std::uint32_t> state_name_to_int;
    std::vector<std::string> int_to_state_name;
    std::vector<int> is_accept_state;

    std::vector<std::vector<Edge>> graph;
};
