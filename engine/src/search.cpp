#include "search.h"
#include "movegen.h"
#include "evaluate.h"
#include "attacks.h"
#include "bitboard.h"
#include <iostream>

namespace ArcKnight::Search {

int quiescence(const Board& board, int alpha, int beta) {
    int stand_pat = Evaluation::evaluate(board);

    if (stand_pat >= beta) {
        return beta;
    }
    if (alpha < stand_pat) {
        alpha = stand_pat;
    }

    MoveList list;
    MoveGen::generate_pseudo_legal(board, list);

    for (int i = 0; i < list.count; ++i) {
        Move move = list.moves[i];
        int flag = Moves::get_flag(move);

        if (flag != FLAG_CAPTURE && flag != FLAG_EP_CAPTURE) {
            continue;
        }

        Board copy = board;
        
        if (!copy.make_move(move)) {
            continue;
        }

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
    int legal_moves = 0;

    for (int i = 0; i < list.count; ++i) {
        Board copy = board;
        
        if (!copy.make_move(list.moves[i])) {
            continue; 
        }
        
        legal_moves++;

        int score = -negamax(copy, depth - 1, -beta, -alpha);

        if (score > max_score) {
            max_score = score;
        }
        if (max_score > alpha) {
            alpha = max_score;
        }
        if (alpha >= beta) {
            break;
        }
    }

    if (legal_moves == 0) {
        Color us = board.side_to_move;
        Color them = (us == WHITE) ? BLACK : WHITE;
        Square king_sq = Bitboards::lsb_index(board.pieces[KING] & board.colors[us]);

        if (Attacks::is_square_attacked(board, king_sq, them)) {
            return -50000 - depth; 
        } else {
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

        int score = -negamax(copy, depth - 1, -50000, 50000);
        
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }
    
    std::cout << "  [Search] Loop finished!" << std::endl;
    return best_move;
}

}