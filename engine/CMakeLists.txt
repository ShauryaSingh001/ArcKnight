add_library(engine STATIC
    src/bitboard.cpp
    src/attacks.cpp
    src/board.cpp
    src/movegen.cpp
    src/evaluate.cpp
    src/search.cpp
    src/uci.cpp
)

target_include_directories(engine PUBLIC inc)