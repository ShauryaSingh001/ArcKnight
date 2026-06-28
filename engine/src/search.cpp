#include "search.h"
#include "movegen.h"
#include "evaluate.h"
#include "attacks.h"
#include "bitboard.h"
#include <iostream>

namespace ArcKnight::Search {

int quiescence(const Board& board, int alpha, int beta) {
    // 1. "Stand Pat" Evaluation
    // What is the score if we just do nothing and don't capture anything?
    int stand_pat = Evaluation::evaluate(board);

    if (stand_pat >= beta) {
        return beta; // The position is already too good, the opponent will avoid it
    }
    if (alpha < stand_pat) {
        alpha = stand_pat; // Update our baseline minimum score
    }

    MoveList list;
    MoveGen::generate_pseudo_legal(board, list);

    for (int i = 0; i < list.count; ++i) {
        Move move = list.moves[i];
        int flag = Moves::get_flag(move);

        // 2. FILTER: Only look at captures! Skip everything else.
        if (flag != FLAG_CAPTURE && flag != FLAG_EP_CAPTURE) {
            continue;
        }

        Board copy = board;
        
        // If the capture is illegal (leaves our King in check), skip it
        if (!copy.make_move(move)) {
            continue;
        }

        // 3. Recursively calculate the next capture in the chain
        int score = -quiescence(copy, -beta, -alpha);

        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}

int negamax(const Board& board, int depth, int alpha, int beta) {
   if (depth == 0) {
        return quiescence(board, alpha, beta);
    }

    MoveList list;
    MoveGen::generate_pseudo_legal(board, list);
    
    int max_score = -999999;
    int legal_moves = 0; // <-- NEW: Keep track of valid moves

    for (int i = 0; i < list.count; ++i) {
        Board copy = board;
        
        // If the move is illegal (leaves King in check), skip it
        if (!copy.make_move(list.moves[i])) {
            continue; 
        }
        
        legal_moves++; // <-- NEW: We found at least one legal move!

        int score = -negamax(copy, depth - 1, -beta, -alpha);

        if (score > max_score) {
            max_score = score;
        }
        if (max_score > alpha) {
            alpha = max_score;
        }
        if (alpha >= beta) {
            break; // Alpha-Beta Pruning
        }
    }

    if (legal_moves == 0) {
        Color us = board.side_to_move;
        Color them = (us == WHITE) ? BLACK : WHITE;
        Square king_sq = Bitboards::lsb_index(board.pieces[KING] & board.colors[us]);

        // Is our King currently under attack?
        if (Attacks::is_square_attacked(board, king_sq, them)) {
            // CHECKMATE!
            // We subtract 'depth' so the engine prefers faster checkmates (e.g. Mate in 1 over Mate in 3)
            return -50000 - depth; 
        } else {
            // STALEMATE!
            return 0; 
        }
    }

    return max_score;
}

Move get_best_move(const Board& board, int depth) {
    std::cout << "  [Search] Generating moves..." << std::endl;
    MoveList list;
    list.count = 0; 
    
    MoveGen::generate_pseudo_legal(board, list);
    std::cout << "  [Search] Generated " << list.count << " pseudo-legal moves." << std::endl;
    
    Move best_move = 0;
    int best_score = -50000;
    
    for (int i = 0; i < list.count; ++i) {
        std::cout << "  [Search] Trying move index " << i << "..." << std::endl;
        Board copy = board;
        Move move = list.moves[i];
        
        if (!copy.make_move(move)) {
            std::cout << "  [Search] Move " << i << " was illegal, skipped." << std::endl;
            continue;
        }
        
       // std::cout << "  [Search] Calling negamax for move " << i << "..." << std::endl;
        int score = -negamax(copy, depth - 1, -50000, 50000);
        // std::cout << "  [Search] Negamax returned score: " << score << std::endl;
        
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }
    
    std::cout << "  [Search] Loop finished!" << std::endl;
    return best_move;
}

} // namespace ArcKnight::Search