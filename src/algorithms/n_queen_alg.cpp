#include "board_games/algorithms/n_queen_alg.hpp"

#include <cstdlib>
#include <ostream>
#include <stdexcept>
#include <vector>

namespace board_games {

bool find_one_sol(char** board, int n, Seq& sol)
{
    int r = sol.size();
    if(r == n)
        return true;

    for(int c = 0; c < n; ++c)
    {
        if(board[r][c] == 'Q')
        {
            bool conflict = false;
            for(int i = 0; i < r; ++i)
            {
                int cc = sol[i];
                if(cc == c || abs(i - r) == abs(cc - c))
                {
                    conflict = true;
                    break;
                }
            }
            if(conflict)
                return false;
            sol.push_back(c);
            bool ret = find_one_sol(board,n,sol);
            if(ret) return true;
            sol.pop_back();
            return false;
        }
        bool conflict = false;
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                if(board[i][j]=='Q' && (j==c||abs(i-r)==abs(j-c)))
                    conflict=true;
        if(conflict) continue;
        for(int i=0;i<r;i++)
        {
            int cc=sol[i];
            if(cc==c||abs(i-r)==abs(cc-c)){conflict=true;break;}
        }
        if(conflict) continue;

        char old=board[r][c];
        board[r][c]='Q';
        sol.push_back(c);
        bool res=find_one_sol(board,n,sol);
        if(res) return true;
        sol.pop_back();
        board[r][c]=old;
    }
    return false;
}

bool find_all_sols(char** board, int n, Seq& sol, SeqSeq& sols)
{
    int r=sol.size();
    if(r==n)
    {
        sols.push_back(sol);
        return true;
    }
    bool ok=false;
    for(int c=0;c<n;c++)
    {
        if(board[r][c]=='Q')
        {
            bool conf=false;
            for(int i=0;i<r;i++)
            {
                int cc=sol[i];
                if(cc==c||abs(i-r)==abs(cc-c)){conf=true;break;}
            }
            if(conf) return false;
            sol.push_back(c);
            bool sub=find_all_sols(board,n,sol,sols);
            ok|=sub;
            sol.pop_back();
            return ok;
        }
        bool conf=false;
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                if(board[i][j]=='Q'&&(j==c||abs(i-r)==abs(j-c))) conf=true;
        if(conf) continue;
        for(int i=0;i<r;i++)
        {
            int cc=sol[i];
            if(cc==c||abs(i-r)==abs(cc-c)){conf=true;break;}
        }
        if(conf) continue;

        char old=board[r][c];
        board[r][c]='Q';
        sol.push_back(c);
        bool sub=find_all_sols(board,n,sol,sols);
        ok|=sub;
        sol.pop_back();
        board[r][c]=old;
    }
    return ok;
}

namespace {

class SolverBoard {
public:
    explicit SolverBoard(int size)
        : cells_(static_cast<std::size_t>(size),
                 std::vector<char>(static_cast<std::size_t>(size), ' ')),
          rows_(static_cast<std::size_t>(size), static_cast<char *>(0))
    {
        for (int row = 0; row < size; ++row) {
            rows_[static_cast<std::size_t>(row)] = cells_[static_cast<std::size_t>(row)].data();
        }
    }

    char **data() { return rows_.data(); }

private:
    std::vector<std::vector<char> > cells_;
    std::vector<char *> rows_;
};

void print_solution(std::ostream &out, int size, const Seq &solution)
{
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            const bool queen = row < static_cast<int>(solution.size()) && solution[row] == column;
            out << (queen ? 'Q' : '.') << (column + 1 == size ? '\n' : ' ');
        }
    }
}

} // namespace

NQueenSolver::NQueenSolver(int size) : size_(size)
{
    if (size <= 0) {
        throw std::invalid_argument("N-Queen solver size must be positive");
    }
}

Seq NQueenSolver::one_solution() const
{
    SolverBoard board(size_);
    Seq solution;
    find_one_sol(board.data(), size_, solution);
    return solution;
}

SeqSeq NQueenSolver::all_solutions() const
{
    SolverBoard board(size_);
    Seq solution;
    SeqSeq solutions;
    find_all_sols(board.data(), size_, solution, solutions);
    return solutions;
}

void NQueenSolver::print_one_solution(std::ostream &out) const
{
    const Seq solution = one_solution();
    if (solution.empty()) {
        out << "No solution.\n";
        return;
    }
    print_solution(out, size_, solution);
}

void NQueenSolver::print_all_solutions(std::ostream &out) const
{
    const SeqSeq solutions = all_solutions();
    out << "Solutions: " << solutions.size() << '\n';
    for (std::size_t index = 0; index < solutions.size(); ++index) {
        print_solution(out, size_, solutions[index]);
        if (index + 1U < solutions.size()) {
            out << '\n';
        }
    }
}

} // namespace board_games
