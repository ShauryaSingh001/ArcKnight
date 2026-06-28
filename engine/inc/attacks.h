#pragma once
#include "types.h"
#include "bitboard.h"

// --- NEW: Tell the compiler this class exists elsewhere ---
namespace ArcKnight {
    class Board; 
}

namespace ArcKnight::Attacks {

extern Bitboard KNIGHT_ATTACKS[64];
extern Bitboard KING_ATTACKS[64];

void init_leaper_attacks();
Bitboard generate_rook_attacks(Square sq, Bitboard occupancy);
Bitboard generate_bishop_attacks(Square sq, Bitboard occupancy);

// Returns true if 'sq' is attacked by any piece belonging to 'attacker_color'
bool is_square_attacked(const Board& board, Square sq, Color attacker_color);

} // namespace ArcKnight::Attacks