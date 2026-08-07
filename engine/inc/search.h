#pragma once
#include "board.h"
#include "move.h"

namespace ArcKnight::Search {

    extern uint64_t nodes_evaluated;    
    Move get_best_move(const Board& board, int depth);

    int negamax(const Board& board, int depth, int alpha, int beta);

}