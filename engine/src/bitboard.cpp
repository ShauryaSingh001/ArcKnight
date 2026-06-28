#include "bitboard.h"

namespace ArcKnight::Bitboards {

void print_bitboard(Bitboard bb) {
    std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << (rank + 1) << " | ";
        for (int file = 0; file < 8; ++file) {
            int square_index = rank * 8 + file;
            if (test_bit(bb, static_cast<Square>(square_index))) {
                std::cout << "1 | ";
            } else {
                std::cout << ". | ";
            }
        }
        std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    }
    std::cout << "    a   b   c   d   e   f   g   h\n\n";
    std::cout << "Hexadecimal representation: 0x" << std::hex << bb << std::dec << "\n\n";
}

} // namespace ArcKnight::Bitboards