#include "board.h"
#include "attacks.h"
#include <iostream>
#include <sstream>

namespace ArcKnight {

void Board::clear() {
    for (int i = 0; i < PIECE_TYPE_NB; ++i) pieces[i] = 0ULL;
    for (int i = 0; i < COLOR_NB; ++i) colors[i] = 0ULL;
    
    side_to_move = WHITE;
    en_passant_sq = SQ_NONE;
    castling_rights = 0;
    half_move_clock = 0;
    full_move_number = 1;
}

PieceType Board::get_piece_at(Square sq) const {
    for (int i = 0; i < PIECE_TYPE_NB; ++i) {
        if (Bitboards::test_bit(pieces[i], sq)) {
            return static_cast<PieceType>(i);
        }
    }
    return PIECE_TYPE_NB; 
}

void Board::print() const {
    const char piece_chars[] = {'P', 'N', 'B', 'R', 'Q', 'K'};
    
    std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << (rank + 1) << " | ";
        for (int file = 0; file < 8; ++file) {
            Square sq = static_cast<Square>(rank * 8 + file);
            PieceType pt = get_piece_at(sq);
            
            if (pt != PIECE_TYPE_NB) {
                char c = piece_chars[pt];
                if (Bitboards::test_bit(colors[BLACK], sq)) {
                    c += 32; 
                }
                std::cout << c << " | ";
            } else {
                std::cout << ". | ";
            }
        }
        std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    }
    std::cout << "    a   b   c   d   e   f   g   h\n\n";
    
    std::cout << "Side to move: " << (side_to_move == WHITE ? "White" : "Black") << "\n";
    std::cout << "Castling rights: " << castling_rights << "\n";
    std::cout << "En Passant square: " << (en_passant_sq == SQ_NONE ? "None" : std::to_string(en_passant_sq)) << "\n\n";
}

void Board::load_fen(const std::string& fen) {
    clear(); 
    std::istringstream ss(fen);
    std::string piece_placement, active_color, castling, en_passant;
    
    ss >> piece_placement >> active_color >> castling >> en_passant >> half_move_clock >> full_move_number;

    int rank = 7; 
    int file = 0; 

    for (char c : piece_placement) {
        if (c == '/') {
            rank--;
            file = 0;
        } else if (isdigit(c)) {
            file += (c - '0'); 
        } else {
            Square sq = static_cast<Square>(rank * 8 + file);
            Color col = isupper(c) ? WHITE : BLACK;
            PieceType pt;

            switch (tolower(c)) {
                case 'p': pt = PAWN; break;
                case 'n': pt = KNIGHT; break;
                case 'b': pt = BISHOP; break;
                case 'r': pt = ROOK; break;
                case 'q': pt = QUEEN; break;
                case 'k': pt = KING; break;
                default: pt = PAWN; break; 
            }

            Bitboards::set_bit(pieces[pt], sq);
            Bitboards::set_bit(colors[col], sq);
            file++;
        }
    }

    side_to_move = (active_color == "w") ? WHITE : BLACK;

    if (castling != "-") {
        for (char c : castling) {
            if (c == 'K') castling_rights |= WHITE_OO;
            if (c == 'Q') castling_rights |= WHITE_OOO;
            if (c == 'k') castling_rights |= BLACK_OO;
            if (c == 'q') castling_rights |= BLACK_OOO;
        }
    }

    if (en_passant != "-") {
        int f = en_passant[0] - 'a';
        int r = en_passant[1] - '1';
        en_passant_sq = static_cast<Square>(r * 8 + f);
    } else {
        en_passant_sq = SQ_NONE;
    }
}

// --- BULLETPROOF MAKE MOVE ---
bool Board::make_move(Move move) {
    Square from = Moves::get_from(move);
    Square to = Moves::get_to(move);
    int flag = Moves::get_flag(move);
    
    PieceType pt = get_piece_at(from);
    if (pt == PIECE_TYPE_NB) return false; // Safety catch
    
    Color us = side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    // Remove piece from starting square
    Bitboards::clear_bit(pieces[pt], from);
    Bitboards::clear_bit(colors[us], from);

    // Guaranteed Capture handling (Nuke anything sitting on the target square)
    PieceType captured_pt = get_piece_at(to);
    if (captured_pt != PIECE_TYPE_NB && Bitboards::test_bit(colors[them], to)) {
        Bitboards::clear_bit(pieces[captured_pt], to);
        Bitboards::clear_bit(colors[them], to);
    }

    // En Passant Capture handling
    if (flag == FLAG_EP_CAPTURE) {
        Square ep_pawn_sq = (us == WHITE) ? static_cast<Square>(to - 8) : static_cast<Square>(to + 8);
        Bitboards::clear_bit(pieces[PAWN], ep_pawn_sq);
        Bitboards::clear_bit(colors[them], ep_pawn_sq);
    }

    // Place piece on target square (Handling Promotions)
    PieceType place_pt = pt;
    if (flag == FLAG_PROMO_QUEEN) place_pt = QUEEN;
    else if (flag == FLAG_PROMO_ROOK) place_pt = ROOK;
    else if (flag == FLAG_PROMO_BISHOP) place_pt = BISHOP;
    else if (flag == FLAG_PROMO_KNIGHT) place_pt = KNIGHT;

    Bitboards::set_bit(pieces[place_pt], to);
    Bitboards::set_bit(colors[us], to);

    // Manage En Passant ghost square
    en_passant_sq = SQ_NONE;
    if (flag == FLAG_DOUBLE_PUSH) {
        en_passant_sq = (us == WHITE) ? static_cast<Square>(from + 8) : static_cast<Square>(from - 8);
    }

    // Swap turns
    side_to_move = them;

    // LEGALITY CHECK
    Square king_sq = Bitboards::lsb_index(pieces[KING] & colors[us]);
    if (king_sq == SQ_NONE) return false; // Safety catch if King vanished
    if (Attacks::is_square_attacked(*this, king_sq, them)) {
        return false; 
    }

    return true; 
}

} // namespace ArcKnight