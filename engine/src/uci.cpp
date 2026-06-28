#include "uci.h"
#include "board.h"
#include "search.h"
#include "movegen.h"
#include "attacks.h"
#include <iostream>
#include <sstream>

namespace ArcKnight::UCI {

Move parse_move(const Board& board, const std::string& move_string) {
    MoveList list;
    MoveGen::generate_pseudo_legal(board, list);
    for (int i = 0; i < list.count; ++i) {
        if (Moves::to_string(list.moves[i]) == move_string) {
            Board copy = board;
            if (copy.make_move(list.moves[i])) return list.moves[i];
        }
    }
    return 0; 
}

void loop() {
    Board board;
    Attacks::init_leaper_attacks();
    std::string line;

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "quit") {
            break; 
        } 
        else if (command == "uci") {
            std::cout << "id name ArcKnight" << std::endl;
            std::cout << "id author Shaurya" << std::endl;
            std::cout << "uciok" << std::endl;
        } 
        else if (command == "isready") {
            std::cout << "readyok" << std::endl;
        } 
        else if (command == "position") {
            std::string token;
            iss >> token;
            if (token == "startpos") {
                board.load_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            } else if (token == "fen") {
                std::string fen;
                while (iss >> token && token != "moves") fen += token + " ";
                board.load_fen(fen);
            }
            while (iss >> token) {
                if (token == "moves") continue;
                Move m = parse_move(board, token);
                if (m != 0) {
                    board.make_move(m);
                } else {
                    // NEW: Scream and crash the engine so the server catches the exact bad move!
                    std::cerr << "FATAL: ArcKnight does not understand the move: " << token << std::endl;
                    exit(1); 
                }
            }
        } 
        else if (command == "go") {
            int target_depth = 4; // A fallback default depth just in case
            std::string token;
            
            // Read the rest of the line to find the depth
            while (iss >> token) {
                if (token == "depth") {
                    iss >> target_depth;
                }
            }

            Move best = Search::get_best_move(board, target_depth);
            std::cout << "bestmove " << Moves::to_string(best) << std::endl;
        }
    }
}

} // namespace ArcKnight::UCI