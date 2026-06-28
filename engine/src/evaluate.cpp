#include "evaluate.h"
#include "bitboard.h"

namespace ArcKnight::Evaluation {

const int PIECE_VALUES[6] = { PAWN_VALUE, KNIGHT_VALUE, BISHOP_VALUE, ROOK_VALUE, QUEEN_VALUE, 0 }; // King has no value

// --- POSITIONAL TABLES (From White's Perspective) ---
// Notice how pawns get massive bonuses (+50) as they reach the 7th rank!
const int PAWN_PST[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
     50, 50, 50, 50, 50, 50, 50, 50,
     10, 10, 20, 30, 30, 20, 10, 10,
      5,  5, 10, 25, 25, 10,  5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5, -5,-10,  0,  0,-10, -5,  5,
      5, 10, 10,-20,-20, 10, 10,  5,
      0,  0,  0,  0,  0,  0,  0,  0
};

// Notice how Knights get negative scores (-50) on the edges, and positive (+20) in the center.
const int KNIGHT_PST[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

// Bishops prefer long diagonals
const int BISHOP_PST[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

const int ROOK_PST[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10, 10, 10, 10, 10,  5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
      0,  0,  0,  5,  5,  0,  0,  0
};

const int QUEEN_PST[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

// King prefers to be tucked away in the corners during the middle game
const int KING_PST[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20
};

const int* PSTS[6] = { PAWN_PST, KNIGHT_PST, BISHOP_PST, ROOK_PST, QUEEN_PST, KING_PST };

int evaluate(const Board& board) {
    int score = 0;

    for (int pt = PAWN; pt <= KING; ++pt) {
        
        // --- Evaluate White ---
        Bitboard white_pieces = board.pieces[pt] & board.colors[WHITE];
        while (white_pieces) {
            Square sq = Bitboards::pop_lsb(white_pieces);
            score += PIECE_VALUES[pt]; 
            score += PSTS[pt][sq]; // Add positional bonus!
        }
        
        // --- Evaluate Black ---
        Bitboard black_pieces = board.pieces[pt] & board.colors[BLACK];
        while (black_pieces) {
            Square sq = Bitboards::pop_lsb(black_pieces);
            score -= PIECE_VALUES[pt];
            
            // Flip the square index to read the table from Black's perspective
            Square flipped_sq = static_cast<Square>(sq ^ 56); 
            score -= PSTS[pt][flipped_sq]; // Subtract positional bonus!
        }
    }

    // A positive score means White is better. 
    // But the search algorithm requires the score to be relative to the side to move!
    return (board.side_to_move == WHITE) ? score : -score;
}

} // namespace ArcKnight::Evaluation