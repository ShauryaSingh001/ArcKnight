#pragma once
#include "types.h"
#include <iostream>

#if defined(_MSC_VER) 
#include <intrin.h>
#endif

namespace ArcKnight::Bitboards {

inline void set_bit(Bitboard& bb, Square sq) {
    bb |= (1ULL << sq);
}

inline bool test_bit(Bitboard bb, Square sq) {
    return (bb & (1ULL << sq)) != 0;
}

inline void clear_bit(Bitboard& bb, Square sq) {
    bb &= ~(1ULL << sq);
}

inline void toggle_bit(Bitboard& bb, Square sq) {
    bb ^= (1ULL << sq);
}

// Counts total active bits using hardware popcnt instruction
inline int count_bits(Bitboard bb) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(bb);
#elif defined(_MSC_VER)
    return static_cast<int>(__popcnt64(bb));
#else
    // Fallback SW population count if no hardware support
    bb -= (bb >> 1) & 0x5555555555555555ULL;
    bb = (bb & 0x3333333333333333ULL) + ((bb >> 2) & 0x3333333333333333ULL);
    return static_cast<int>((((bb + (bb >> 4)) & 0xF0F0F0F0F0F0F0FFULL) * 0x0101010101010101ULL) >> 56);
#endif
}

// Bitscan Forward: Finds the index of the least significant set bit (LSB)
inline Square lsb_index(Bitboard bb) {
    if (bb == 0) return SQ_NONE;
#if defined(__GNUC__) || defined(__clang__)
    return static_cast<Square>(__builtin_ctzll(bb));
#elif defined(_MSC_VER)
    unsigned long index;
    _BitScanForward64(&index, bb);
    return static_cast<Square>(index);
#endif
}

// Removes the LSB and returns its original index (Crucial for iterating through moves)
inline Square pop_lsb(Bitboard& bb) {
    Square index = lsb_index(bb);
    bb &= bb - 1; // Clears the lowest set bit
    return index;
}

// Visual print layout utility for terminal debugging
void print_bitboard(Bitboard bb);

} // namespace ArcKnight::Bitboards