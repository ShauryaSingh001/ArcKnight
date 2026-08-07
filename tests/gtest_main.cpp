#include <chrono>
#include <iostream>
#include <gtest/gtest.h>
#include "move.h"
#include "search.h"
#include "board.h"


using namespace ArcKnight;

TEST(MoveEncodingTest, EncodesAndDecodesCorrectly) {

    Square from = SQ_E2;
    Square to = SQ_E4;
    int flag = FLAG_DOUBLE_PUSH;

    Move m = Moves::encode(from, to, flag);

    EXPECT_EQ(Moves::get_from(m), SQ_E2);
    EXPECT_EQ(Moves::get_to(m), SQ_E4);
    EXPECT_EQ(Moves::get_flag(m), FLAG_DOUBLE_PUSH);
}

TEST(EnginePerformance, BenchmarksNodesPerSecond) {
    ArcKnight::Board board; 

    board.load_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    int target_depth = 6;

    ArcKnight::Search::nodes_evaluated = 0; 
    
    auto start_time = std::chrono::high_resolution_clock::now();

    ArcKnight::Search::negamax(board, target_depth, -50000, 50000);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    double seconds = elapsed.count();

    uint64_t nps = static_cast<uint64_t>(ArcKnight::Search::nodes_evaluated / seconds);
    
    std::cout << "[ METRICS  ] Depth: " << target_depth << " ply\n";
    std::cout << "[ METRICS  ] Nodes: " << ArcKnight::Search::nodes_evaluated << "\n";
    std::cout << "[ METRICS  ] Time : " << seconds << "s\n";
    std::cout << "[ METRICS  ] Speed: " << nps << " NPS\n";

    EXPECT_GT(nps, 0); 
}