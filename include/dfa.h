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

    void add_state(const std::string& state_name, bool accept_state, 
                                                  bool start_state = false);
    void add_edge(const std::string& from, const std::string& to, 
                                           const std::string& symbol);

    void minimize();
    void draw_in_DOT(std::ostream& out) const;

private:
    uint32_t number_of_states;
    uint32_t start_state_number;

    std::unordered_map<std::string, std::uint32_t> state_name_to_int;
    std::vector<std::string> int_to_state_name;
    std::vector<int> is_accept_state;

    struct Edge {
        std::uint32_t destination;
        std::string symbol;
        bool operator<(const Edge& other) const;
    };

    std::vector<std::vector<Edge>> graph;
    
    void dfs(uint32_t vert, std::vector<int>& used);
    void remove_unreachable_states();
    
    struct PairEdge {
        std::uint32_t destination_1;
        std::uint32_t destination_2;
        std::string symbol;
    };

    void dfs_on_pairs(uint32_t v1, uint32_t v2, 
                      std::vector<std::vector<char>>& used, 
                      std::vector<std::vector<std::vector<PairEdge>>>& gr);
    void build_disting_matrix(std::vector<std::vector<char>>& disting);
    void unite_states();
};
