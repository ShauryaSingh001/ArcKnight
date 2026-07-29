#include "movegen.h"
#include "bitboard.h"

namespace ArcKnight::MoveGen {

constexpr Bitboard RANK_3 = 0x0000000000FF0000ULL;
constexpr Bitboard RANK_6 = 0x0000FF0000000000ULL;
constexpr Bitboard NOT_A_FILE = ~FILE_A;
constexpr Bitboard NOT_H_FILE = ~FILE_H;

void generate_pseudo_legal(const Board& board, MoveList& list) {
    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;
    
    Bitboard our_pieces = board.colors[us];
    Bitboard enemy_pieces = board.colors[them];
    Bitboard occupancy = our_pieces | enemy_pieces;

    Bitboard pawns = board.pieces[PAWN] & our_pieces;
    
    if (us == WHITE) {
        Bitboard push_one = (pawns << 8) & ~occupancy;
        Bitboard p1 = push_one;
        while (p1) {
            Square to = Bitboards::pop_lsb(p1);
            Square from = static_cast<Square>(to - 8);
            if (to >= SQ_A8) { 
                list.add(Moves::encode(from, to, FLAG_PROMO_QUEEN));
                list.add(Moves::encode(from, to, FLAG_PROMO_ROOK));
                list.add(Moves::encode(from, to, FLAG_PROMO_BISHOP));
                list.add(Moves::encode(from, to, FLAG_PROMO_KNIGHT));
            } else {
                list.add(Moves::encode(from, to, FLAG_QUIET));
            }
        }
        
        Bitboard push_two = ((push_one & RANK_3) << 8) & ~occupancy;
        Bitboard p2 = push_two;
        while (p2) {
            Square to = Bitboards::pop_lsb(p2);
            Square from = static_cast<Square>(to - 16);
            list.add(Moves::encode(from, to, FLAG_DOUBLE_PUSH));
        }
        Bitboard caps_right = (pawns << 9) & NOT_A_FILE & enemy_pieces;
        while (caps_right) {
            Square to = Bitboards::pop_lsb(caps_right);
            Square from = static_cast<Square>(to - 9);
            if (to >= SQ_A8) { // Capture + Promotion
                list.add(Moves::encode(from, to, FLAG_PROMO_QUEEN));
                list.add(Moves::encode(from, to, FLAG_PROMO_ROOK));
                list.add(Moves::encode(from, to, FLAG_PROMO_BISHOP));
                list.add(Moves::encode(from, to, FLAG_PROMO_KNIGHT));
            } else {
                list.add(Moves::encode(from, to, FLAG_CAPTURE));
            }
        }
        Bitboard caps_left = (pawns << 7) & NOT_H_FILE & enemy_pieces;
        while (caps_left) {
            Square to = Bitboards::pop_lsb(caps_left);
            Square from = static_cast<Square>(to - 7);
            if (to >= SQ_A8) {
                list.add(Moves::encode(from, to, FLAG_PROMO_QUEEN));
                list.add(Moves::encode(from, to, FLAG_PROMO_ROOK));
                list.add(Moves::encode(from, to, FLAG_PROMO_BISHOP));
                list.add(Moves::encode(from, to, FLAG_PROMO_KNIGHT));
            } else {
                list.add(Moves::encode(from, to, FLAG_CAPTURE));
            }
        }
        
        if (board.en_passant_sq != SQ_NONE) {
            Bitboard ep_bb = 1ULL << board.en_passant_sq;
            Bitboard ep_left = (pawns << 7) & NOT_H_FILE & ep_bb;
            Bitboard ep_right = (pawns << 9) & NOT_A_FILE & ep_bb;
            if (ep_left) list.add(Moves::encode(static_cast<Square>(board.en_passant_sq - 7), board.en_passant_sq, FLAG_EP_CAPTURE));
            if (ep_right) list.add(Moves::encode(static_cast<Square>(board.en_passant_sq - 9), board.en_passant_sq, FLAG_EP_CAPTURE));
        }

    } else {
        // --- BLACK PAWNS ---
        Bitboard push_one = (pawns >> 8) & ~occupancy;
        Bitboard p1 = push_one;
        while (p1) {
            Square to = Bitboards::pop_lsb(p1);
            Square from = static_cast<Square>(to + 8);
            if (to <= SQ_H1) { 
                list.add(Moves::encode(from, to, FLAG_PROMO_QUEEN));
                list.add(Moves::encode(from, to, FLAG_PROMO_ROOK));
                list.add(Moves::encode(from, to, FLAG_PROMO_BISHOP));
                list.add(Moves::encode(from, to, FLAG_PROMO_KNIGHT));
            } else {
                list.add(Moves::encode(from, to, FLAG_QUIET));
            }
        }
        
        Bitboard push_two = ((push_one & RANK_6) >> 8) & ~occupancy;
        Bitboard p2 = push_two;
        while (p2) {
            Square to = Bitboards::pop_lsb(p2);
            Square from = static_cast<Square>(to + 16);
            list.add(Moves::encode(from, to, FLAG_DOUBLE_PUSH));
        }

        Bitboard caps_right = (pawns >> 7) & NOT_A_FILE & enemy_pieces;
        while (caps_right) {
            Square to = Bitboards::pop_lsb(caps_right);
            Square from = static_cast<Square>(to + 7);
            if (to <= SQ_H1) { 
                list.add(Moves::encode(from, to, FLAG_PROMO_QUEEN));
                list.add(Moves::encode(from, to, FLAG_PROMO_ROOK));
                list.add(Moves::encode(from, to, FLAG_PROMO_BISHOP));
                list.add(Moves::encode(from, to, FLAG_PROMO_KNIGHT));
            } else {
                list.add(Moves::encode(from, to, FLAG_CAPTURE));
            }
        }

        Bitboard caps_left = (pawns >> 9) & NOT_H_FILE & enemy_pieces;
        while (caps_left) {
            Square to = Bitboards::pop_lsb(caps_left);
            Square from = static_cast<Square>(to + 9);
            if (to <= SQ_H1) {
                list.add(Moves::encode(from, to, FLAG_PROMO_QUEEN));
                list.add(Moves::encode(from, to, FLAG_PROMO_ROOK));
                list.add(Moves::encode(from, to, FLAG_PROMO_BISHOP));
                list.add(Moves::encode(from, to, FLAG_PROMO_KNIGHT));
            } else {
                list.add(Moves::encode(from, to, FLAG_CAPTURE));
            }
        }
        
        if (board.en_passant_sq != SQ_NONE) {
            Bitboard ep_bb = 1ULL << board.en_passant_sq;
            Bitboard ep_left = (pawns >> 9) & NOT_H_FILE & ep_bb;
            Bitboard ep_right = (pawns >> 7) & NOT_A_FILE & ep_bb;
            if (ep_left) list.add(Moves::encode(static_cast<Square>(board.en_passant_sq + 9), board.en_passant_sq, FLAG_EP_CAPTURE));
            if (ep_right) list.add(Moves::encode(static_cast<Square>(board.en_passant_sq + 7), board.en_passant_sq, FLAG_EP_CAPTURE));
        }
    }

    // --- Generate Knight Moves ---
    Bitboard knights = board.pieces[KNIGHT] & our_pieces;
    while (knights) {
        Square from = Bitboards::pop_lsb(knights);
        Bitboard attacks = Attacks::KNIGHT_ATTACKS[from] & ~our_pieces;
        while (attacks) {
            Square to = Bitboards::pop_lsb(attacks);
            int flag = Bitboards::test_bit(enemy_pieces, to) ? FLAG_CAPTURE : FLAG_QUIET;
            list.add(Moves::encode(from, to, flag));
        }
    }

    // --- Generate King Moves ---
    Bitboard kings = board.pieces[KING] & our_pieces;
    while (kings) {
        Square from = Bitboards::pop_lsb(kings);
        Bitboard attacks = Attacks::KING_ATTACKS[from] & ~our_pieces;
        while (attacks) {
            Square to = Bitboards::pop_lsb(attacks);
            int flag = Bitboards::test_bit(enemy_pieces, to) ? FLAG_CAPTURE : FLAG_QUIET;
            list.add(Moves::encode(from, to, flag));
        }
    }

    // --- Generate Bishop Moves ---
    Bitboard bishops = board.pieces[BISHOP] & our_pieces;
    while (bishops) {
        Square from = Bitboards::pop_lsb(bishops);
        Bitboard attacks = Attacks::generate_bishop_attacks(from, occupancy) & ~our_pieces;
        while (attacks) {
            Square to = Bitboards::pop_lsb(attacks);
            int flag = Bitboards::test_bit(enemy_pieces, to) ? FLAG_CAPTURE : FLAG_QUIET;
            list.add(Moves::encode(from, to, flag));
        }
    }

    // --- Generate Rook Moves ---
    Bitboard rooks = board.pieces[ROOK] & our_pieces;
    while (rooks) {
        Square from = Bitboards::pop_lsb(rooks);
        Bitboard attacks = Attacks::generate_rook_attacks(from, occupancy) & ~our_pieces;
        while (attacks) {
            Square to = Bitboards::pop_lsb(attacks);
            int flag = Bitboards::test_bit(enemy_pieces, to) ? FLAG_CAPTURE : FLAG_QUIET;
            list.add(Moves::encode(from, to, flag));
        }
    }

    // --- Generate Queen Moves ---
    Bitboard queens = board.pieces[QUEEN] & our_pieces;
    while (queens) {
        Square from = Bitboards::pop_lsb(queens);
        Bitboard attacks = (Attacks::generate_bishop_attacks(from, occupancy) | 
                            Attacks::generate_rook_attacks(from, occupancy)) & ~our_pieces;
        while (attacks) {
            Square to = Bitboards::pop_lsb(attacks);
            int flag = Bitboards::test_bit(enemy_pieces, to) ? FLAG_CAPTURE : FLAG_QUIET;
            list.add(Moves::encode(from, to, flag));
        }
    }
}

}