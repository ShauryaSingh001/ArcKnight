#pragma once
#include "types.h"
#include "bitboard.h"
#include "move.h"
#include <string>

namespace ArcKnight {

// Castling rights bit masks
enum CastlingRights {
    WHITE_OO  = 1,
    WHITE_OOO = 2,
    BLACK_OO  = 4,
    BLACK_OOO = 8
};

class Board {
public:
    Bitboard pieces[PIECE_TYPE_NB];
    Bitboard colors[COLOR_NB];

    Color side_to_move;
    Square en_passant_sq;
    int castling_rights;
    int half_move_clock; 
    int full_move_number;

    void clear();
    void print() const;
    PieceType get_piece_at(Square sq) const;
    
    void load_fen(const std::string& fen);
    bool make_move(Move move);
};

}