#pragma once
#include "board.h"

namespace ArcKnight::Evaluation {

constexpr int PAWN_VALUE   = 100;
constexpr int KNIGHT_VALUE = 300;
constexpr int BISHOP_VALUE = 300;
constexpr int ROOK_VALUE   = 500;
constexpr int QUEEN_VALUE  = 900;

// --- NEW: Piece-Square Tables ---
extern const int PAWN_PST[64];
extern const int KNIGHT_PST[64];
extern const int BISHOP_PST[64];
extern const int ROOK_PST[64];
extern const int QUEEN_PST[64];
extern const int KING_PST[64];

int evaluate(const Board& board);

} // namespace ArcKnight::Evaluation