#pragma once
#include "board.h"
#include "move.h"
#include "attacks.h"

namespace ArcKnight::MoveGen {

// Generates all pseudo-legal moves for the side to move
void generate_pseudo_legal(const Board& board, MoveList& list);

} // namespace ArcKnight::MoveGen