# ♟️ ArcKnight

> **A full-stack chess engine built from scratch in C++, featuring a custom search engine, a Node.js backend, and a Cyberpunk-inspired web interface.**

![ArcKnight UI](assets/arcKnight-ui.png)

## 🚀 Overview

ArcKnight is a chess engine I built to understand how modern chess engines work under the hood while improving my skills in systems programming, algorithms, and full-stack development.

Instead of relying on existing engines like Stockfish, I implemented my own engine in **C++**, focusing on efficient move generation and game-tree search. The engine communicates with a **Node.js** backend that serves a responsive Cyberpunk-inspired web interface where users can play against the engine in real time.

The goal of this project was not just to build a playable chess game, but to explore the algorithms and optimizations that allow computers to evaluate millions of possible positions efficiently.

---

# ✨ Features

* ♟️ Custom chess engine written entirely in C++
* ⚡ Bitboard-based board representation for high-performance move generation
* 🧠 Negamax search with Alpha-Beta pruning
* 🎯 Quiescence Search to reduce the Horizon Effect
* 🌐 Node.js backend communicating with the engine using child processes
* 🎨 Custom Cyberpunk-inspired responsive interface
* ⏱️ Real-time chess clocks
* 📜 Live move-history terminal
* ✨ Animated move highlights and check notifications

---

# 🏗️ Architecture

```
Frontend (HTML/CSS/JavaScript)
            │
            ▼
      Node.js Backend
            │
     child_process API
            │
            ▼
      C++ Chess Engine
```

The application is divided into three independent layers to keep the engine, backend, and UI cleanly separated.

---

# 🧠 C++ Chess Engine

The engine is responsible for move generation, board evaluation, and selecting the strongest move.

### Efficient Board Representation

The board is represented using **Bitboards (64-bit integers)**, allowing move generation to rely almost entirely on fast bitwise operations instead of arrays.

This approach dramatically improves both speed and memory efficiency.

### Search Algorithm

The engine searches positions using:

* Negamax
* Alpha-Beta Pruning

Alpha-Beta pruning eliminates branches that cannot influence the final decision, allowing the engine to search significantly deeper than a naïve minimax implementation.

### Quiescence Search

To avoid tactical mistakes caused by the **Horizon Effect**, the engine extends its search through forcing capture sequences before evaluating a position.

This results in much more stable tactical play.

### Position Evaluation

The evaluation function scores positions using multiple heuristics, including:

* Material balance
* Piece-square tables
* Mobility
* King safety
* Pawn structure

---

# 🌐 Node.js Backend

The backend acts as a bridge between the browser and the C++ engine.

Its responsibilities include:

* Spawning the compiled C++ executable
* Maintaining communication through standard input/output
* Receiving moves from the frontend
* Returning the engine's best calculated move
* Exposing a lightweight REST API

This architecture keeps the engine completely independent from the frontend, making it easy to swap interfaces or extend functionality in the future.

---

# 🎨 Frontend

The frontend was designed with a futuristic **Cyberpunk / Neon Tokyo** aesthetic.

Features include:

* Responsive design
* Animated move highlights
* Live chess clocks
* Terminal-inspired move history
* Check and checkmate alerts
* Smooth UI micro-interactions

The interface uses:

* HTML5
* CSS3
* JavaScript
* chess.js (move legality)
* chessboard.js (board rendering)

> **Note:** `chess.js` is used only for client-side move validation. All move evaluation and decision-making are performed by the custom C++ engine.

---

# 📁 Project Structure

```
ArcKnight/
│
├── engine/
│   ├── bitboard/
│   ├── movegen/
│   ├── search/
│   ├── evaluation/
│   └── main.cpp
│
├── backend/
│   ├── server.js
│   └── package.json
│
├── frontend/
│   ├── index.html
│   ├── style.css
│   ├── script.js
│   └── assets/
│
├── CMakeLists.txt
└── README.md
```

---

# 🛠️ Tech Stack

| Layer     | Technologies            |
| --------- | ----------------------- |
| Engine    | C++17, CMake            |
| Backend   | Node.js, Express        |
| Frontend  | HTML5, CSS3, JavaScript |
| Libraries | chess.js, chessboard.js |

---

# 🚀 Future Improvements

Planned enhancements include:

* Move Ordering (MVV-LVA, Killer Moves, History Heuristic)
* Transposition Tables using Zobrist Hashing
* Iterative Deepening Search
* Opening Book
* Endgame Tablebases
* UCI Protocol support
* Adjustable engine difficulty
* Performance benchmarking and profiling

---

# ⚙️ Running Locally

## 1. Build the engine

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## 2. Install backend dependencies

```bash
npm install
```

## 3. Start the backend

```bash
node server.js
```

## 4. Play

Open the frontend in your browser and start your first game as White.

---