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
    console.log(`\n[Server] 📥 Received request for moves: "${moveHistory}"`);
    
    // Spawn the engine
    const engine = spawn(ENGINE_PATH);
    let engineOutput = "";
    let handled = false;

    // 1. Monitor real-time output
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

    // 2. Catch stderr (C++ runtime errors/assertions)
    engine.stderr.on('data', (data) => {
        console.error(`[Engine Stderr ❌]: ${data.toString()}`);
    });

    // 3. Handle unexpected engine closures/crashes
    engine.on('close', (code) => {
        console.log(`[Server] 💀 Engine process exited with code ${code}`);
        if (!handled) {
            res.status(500).json({ error: `Engine died unexpectedly with code ${code}` });
            handled = true;
        }
    });

    // Send standard test commands down the pipe
    engine.stdin.write("uci\n");
    engine.stdin.write("isready\n");
    
    // If moveHistory is empty, just ask it to think from startpos
    if (moveHistory) {
        engine.stdin.write(`position startpos moves ${moveHistory}\n`);
    } else {
        engine.stdin.write(`position startpos\n`);
    }
    
    engine.stdin.write("go depth 1\n"); // Specifying a shallow depth prevents infinite loops!
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`🚀 Diagnostics Server running on http://localhost:${PORT}`);
});