#ifndef BOARD_GAMES_ALGORITHMS_N_QUEEN_ALG_HPP
#define BOARD_GAMES_ALGORITHMS_N_QUEEN_ALG_HPP

#include <iosfwd>
#include <vector>

namespace board_games {

using Seq = std::vector<int>;
using SeqSeq = std::vector<std::vector<int> >;

bool find_one_sol(char **board, int n, Seq &sol);
bool find_all_sols(char **board, int n, Seq &sol, SeqSeq &sols);

class NQueenSolver {
public:
    explicit NQueenSolver(int size);

    int size() const noexcept { return size_; }
    Seq one_solution() const;
    SeqSeq all_solutions() const;
    void print_one_solution(std::ostream &out) const;
    void print_all_solutions(std::ostream &out) const;

private:
    int size_;
};

} // namespace board_games

#endif
