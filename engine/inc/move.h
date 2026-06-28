#pragma once
#include "types.h"
#include <cstdint>
#include <string>

namespace ArcKnight {

// Move Flags (4 bits max, so 0 to 15)
enum MoveFlag : int {
    FLAG_QUIET          = 0,
    FLAG_DOUBLE_PUSH    = 1,
    FLAG_KING_CASTLE    = 2,
    FLAG_QUEEN_CASTLE   = 3,
    FLAG_CAPTURE        = 4,
    FLAG_EP_CAPTURE     = 5,
    FLAG_PROMO_KNIGHT   = 8,
    FLAG_PROMO_BISHOP   = 9,
    FLAG_PROMO_ROOK     = 10,
    FLAG_PROMO_QUEEN    = 11
};

// 16-bit integer to hold everything
using Move = std::uint16_t;

namespace Moves {

// --- ENCODING ---
// Shifts the 'To' square by 6 bits, and the 'Flags' by 12 bits, leaving 'From' at the bottom.
inline Move encode(Square from, Square to, int flag = FLAG_QUIET) {
    return static_cast<Move>(from | (to << 6) | (flag << 12));
}

// --- DECODING ---
// Use bitwise AND (&) with a mask (0x3F = 63, which is 6 bits of 1s) to isolate the data.
inline Square get_from(Move move) {
    return static_cast<Square>(move & 0x3F);
}

inline Square get_to(Move move) {
    return static_cast<Square>((move >> 6) & 0x3F);
}

inline int get_flag(Move move) {
    return (move >> 12) & 0x0F;
}

// --- UTILITY ---
// Converts a move like 12 to 28 into standard algebraic notation like "e2e4"
inline std::string to_string(Move move) {
    Square f = get_from(move);
    Square t = get_to(move);
    
    std::string s = "";
    s += char('a' + (f % 8)); // from file
    s += char('1' + (f / 8)); // from rank
    s += char('a' + (t % 8)); // to file
    s += char('1' + (t / 8)); // to rank
    
    // Add promotion character if applicable
    int flag = get_flag(move);
    if (flag == FLAG_PROMO_QUEEN) s += 'q';
    else if (flag == FLAG_PROMO_ROOK) s += 'r';
    else if (flag == FLAG_PROMO_BISHOP) s += 'b';
    else if (flag == FLAG_PROMO_KNIGHT) s += 'n';
    
    return s;
}

} // namespace Moves

// --- MOVELIST ---
// A pre-allocated array to hold moves during generation (much faster than std::vector)
struct MoveList {
    Move moves[256]; // 256 is higher than the max possible legal moves in any chess position
    int count = 0;

    inline void add(Move move) {
        moves[count++] = move;
    }
};

} // namespace ArcKnight