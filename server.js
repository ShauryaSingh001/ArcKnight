const express = require('express');
const { spawn } = require('child_process');
const cors = require('cors');
const path = require('path');

const app = express();
app.use(cors());
app.use(express.json());

const ENGINE_PATH = path.join(__dirname, 'build', 'tests', 'arc_test.exe');

app.post('/api/move', (req, res) => {
    const moveHistory = req.body.moves || ""; 
    const isUntimed = req.body.isUntimed || false;
    
    console.log(`\n[Server] 📥 Received request for moves: "${moveHistory}"`);
    console.log(`[Server] ⏱️ Zen Mode (Untimed): ${isUntimed}`);

    const engine = spawn(ENGINE_PATH);
    let engineOutput = "";
    let handled = false;

    engine.stdout.on('data', (data) => {
        const chunk = data.toString();
        engineOutput += chunk;
        console.log(`[Engine Stdout]: ${chunk.trim()}`);

        const match = engineOutput.match(/bestmove\s+([a-h][1-8][a-h][1-8][qrbn]?)/);
        if (match && !handled) {
            handled = true;
            res.json({ bestmove: match[1] });
            engine.kill();
        }
    });

    engine.stderr.on('data', (data) => {
        console.error(`[Engine Stderr ❌]: ${data.toString()}`);
    });

    engine.on('close', (code) => {
        console.log(`[Server] 💀 Engine process exited with code ${code}`);
        if (!handled) {
            res.status(500).json({ error: `Engine died unexpectedly with code ${code}` });
            handled = true;
        }
    });

    engine.stdin.write("uci\n");
    engine.stdin.write("isready\n");
    
    if (moveHistory) {
        engine.stdin.write(`position startpos moves ${moveHistory}\n`);
    } else {
        engine.stdin.write(`position startpos\n`);
    }
    
    // Engine Search Command Logic
    if (isUntimed) {
        engine.stdin.write("go depth 6\n"); // Deeper search for untimed mode
    } else {
        engine.stdin.write("go depth 5\n"); 
    }
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`🚀 Diagnostics Server running on http://localhost:${PORT}`);
});