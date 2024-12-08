import express from 'express';
import { createServer } from 'http';
import { WebSocketServer } from 'ws';
import { Client } from 'node-osc';

const app = express();
const server = createServer(app);
const wss = new WebSocketServer({ server });

// Enable JSON body parsing
app.use(express.json());

// Enable CORS
app.use((req, res, next) => {
    res.header('Access-Control-Allow-Origin', '*');
    res.header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS');
    res.header('Access-Control-Allow-Headers', 'Content-Type');
    next();
});

// Serve static files from public directory
app.use(express.static('public'));

// Create OSC client to send messages to OpenFrameworks
const oscClient = new Client('127.0.0.1', 57120);

// Add endpoint for sending OSC messages
app.post('/send-trigger', (req, res) => {
    const state = req.body.state;
    
    // Send OSC message to OpenFrameworks
    oscClient.send('/trigger', state, () => {
        console.log(`Sent state ${state} to OpenFrameworks`);
        
        // Broadcast to all WebSocket clients
        wss.clients.forEach(client => {
            client.send(JSON.stringify({
                type: 'trigger-sent',
                state: state,
                timestamp: new Date().toISOString()
            }));
        });
        
        res.json({ 
            success: true,
            message: `State ${state} sent to OpenFrameworks`,
            state: state
        });
    });
});

// WebSocket connection handling
wss.on('connection', (ws) => {
    console.log('Client connected');

    // Send initial connection confirmation
    ws.send(JSON.stringify({
        type: 'connection',
        status: 'connected',
        timestamp: new Date().toISOString()
    }));

    // Handle client disconnection
    ws.on('close', () => {
        console.log('Client disconnected');
    });

    // Handle errors
    ws.on('error', (error) => {
        console.error('WebSocket error:', error);
    });
});

// Error handling middleware
app.use((err, req, res, next) => {
    console.error('Server error:', err);
    res.status(500).json({
        success: false,
        message: 'Internal server error',
        error: err.message
    });
});

// Start server
const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
    console.log(`WebSocket server is ready`);
    console.log(`OSC client connected to port 57120`);
});

// Graceful shutdown handling
process.on('SIGTERM', () => {
    console.log('SIGTERM signal received: closing HTTP server');
    server.close(() => {
        console.log('HTTP server closed');
        oscClient.close();
        process.exit(0);
    });
});

process.on('SIGINT', () => {
    console.log('SIGINT signal received: closing HTTP server');
    server.close(() => {
        console.log('HTTP server closed');
        oscClient.close();
        process.exit(0);
    });
});