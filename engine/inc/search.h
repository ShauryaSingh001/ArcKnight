#pragma once
#include "board.h"
#include "move.h"

namespace ArcKnight::Search {

// The main interface to start the engine thinking
Move get_best_move(const Board& board, int depth);

} // namespace ArcKnight::Search