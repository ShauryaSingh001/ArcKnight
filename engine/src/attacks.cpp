#include "attacks.h"
#include "board.h"

namespace ArcKnight::Attacks {

Bitboard KNIGHT_ATTACKS[64];
Bitboard KING_ATTACKS[64];

constexpr Bitboard NOT_A_FILE = ~FILE_A;
constexpr Bitboard NOT_AB_FILE = ~(FILE_A | FILE_B);
constexpr Bitboard NOT_H_FILE = ~FILE_H;
constexpr Bitboard NOT_GH_FILE = ~(FILE_G | FILE_H);

void init_leaper_attacks() {
    for (int sq = 0; sq < 64; ++sq) {
        Bitboard piece = 0ULL;
        Bitboards::set_bit(piece, static_cast<Square>(sq));

        // --- Calculate Knight Attacks ---
        Bitboard knight_moves = 0ULL;
        knight_moves |= (piece << 17) & NOT_A_FILE;
        knight_moves |= (piece << 15) & NOT_H_FILE;
        knight_moves |= (piece >> 15) & NOT_A_FILE;
        knight_moves |= (piece >> 17) & NOT_H_FILE;
        knight_moves |= (piece << 10) & NOT_AB_FILE;
        knight_moves |= (piece << 6) & NOT_GH_FILE;
        knight_moves |= (piece >> 6) & NOT_AB_FILE;
        knight_moves |= (piece >> 10) & NOT_GH_FILE;
        KNIGHT_ATTACKS[sq] = knight_moves;

        // --- Calculate King Attacks ---
        Bitboard king_moves = 0ULL;
        king_moves |= (piece << 8); // Up
        king_moves |= (piece >> 8); // Down
        king_moves |= (piece << 1) & NOT_A_FILE; // Left
        king_moves |= (piece >> 1) & NOT_H_FILE; // Right
        king_moves |= (piece << 9) & NOT_A_FILE; // Up-Left
        king_moves |= (piece << 7) & NOT_H_FILE; // Up-Right
        king_moves |= (piece >> 7) & NOT_A_FILE; // Down-Left
        king_moves |= (piece >> 9) & NOT_H_FILE; // Down-Right
        KING_ATTACKS[sq] = king_moves;
    }
}

Bitboard generate_rook_attacks(Square sq, Bitboard occupancy) {
    Bitboard attacks = 0ULL;
    int r = sq / 8;
    int f = sq % 8;

    for (int rank = r + 1; rank <= 7; ++rank) {
        Square target = static_cast<Square>(rank * 8 + f);
        Bitboards::set_bit(attacks, target);
        if (Bitboards::test_bit(occupancy, target)) break;
    }
    for (int rank = r - 1; rank >= 0; --rank) {
        Square target = static_cast<Square>(rank * 8 + f);
        Bitboards::set_bit(attacks, target);
        if (Bitboards::test_bit(occupancy, target)) break;
    }
    for (int file = f + 1; file <= 7; ++file) {
        Square target = static_cast<Square>(r * 8 + file);
        Bitboards::set_bit(attacks, target);
        if (Bitboards::test_bit(occupancy, target)) break;
    }
    for (int file = f - 1; file >= 0; --file) {
        Square target = static_cast<Square>(r * 8 + file);
        Bitboards::set_bit(attacks, target);
        if (Bitboards::test_bit(occupancy, target)) break;
    }

    return attacks;
}

Bitboard generate_bishop_attacks(Square sq, Bitboard occupancy) {
    Bitboard attacks = 0ULL;
    int r = sq / 8;
    int f = sq % 8;

    for (int rank = r + 1, file = f + 1; rank <= 7 && file <= 7; ++rank, ++file) {
        Square target = static_cast<Square>(rank * 8 + file);
        Bitboards::set_bit(attacks, target);
        if (Bitboards::test_bit(occupancy, target)) break;
    }
    for (int rank = r + 1, file = f - 1; rank <= 7 && file >= 0; ++rank, --file) {
        Square target = static_cast<Square>(rank * 8 + file);
        Bitboards::set_bit(attacks, target);
        if (Bitboards::test_bit(occupancy, target)) break;
    }
    for (int rank = r - 1, file = f + 1; rank >= 0 && file <= 7; --rank, ++file) {
        Square target = static_cast<Square>(rank * 8 + file);
        Bitboards::set_bit(attacks, target);
        if (Bitboards::test_bit(occupancy, target)) break;
    }
    for (int rank = r - 1, file = f - 1; rank >= 0 && file >= 0; --rank, --file) {
        Square target = static_cast<Square>(rank * 8 + file);
        Bitboards::set_bit(attacks, target);
        if (Bitboards::test_bit(occupancy, target)) break;
    }

    return attacks;
}

bool is_square_attacked(const Board& board, Square sq, Color attacker_color) {
    Bitboard occupancy = board.colors[WHITE] | board.colors[BLACK];
    Bitboard attackers = board.colors[attacker_color];

    // 1. Attacked by Pawns? (We reverse the pawn attack direction)
    Bitboard pawn_attacks = 0ULL;
    if (attacker_color == WHITE) { // White pawns attack "up", so we look "down" from the square
        pawn_attacks |= (1ULL << sq) >> 7 & NOT_A_FILE;
        pawn_attacks |= (1ULL << sq) >> 9 & NOT_H_FILE;
    } else { // Black pawns attack "down", so we look "up"
        pawn_attacks |= (1ULL << sq) << 9 & NOT_A_FILE;
        pawn_attacks |= (1ULL << sq) << 7 & NOT_H_FILE;
    }
    if (pawn_attacks & board.pieces[PAWN] & attackers) return true;

    // 2. Attacked by Knights?
    if (KNIGHT_ATTACKS[sq] & board.pieces[KNIGHT] & attackers) return true;

    // 3. Attacked by Kings?
    if (KING_ATTACKS[sq] & board.pieces[KING] & attackers) return true;

    // 4. Attacked by Bishops or Queens?
    Bitboard diagonal_attackers = (board.pieces[BISHOP] | board.pieces[QUEEN]) & attackers;
    if (generate_bishop_attacks(sq, occupancy) & diagonal_attackers) return true;

    // 5. Attacked by Rooks or Queens?
    Bitboard straight_attackers = (board.pieces[ROOK] | board.pieces[QUEEN]) & attackers;
    if (generate_rook_attacks(sq, occupancy) & straight_attackers) return true;

    return false;
}

} // namespace ArcKnight::Attacks