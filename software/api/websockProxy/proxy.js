import WebSocket from 'ws';
import express from 'express';
import { WebSocketServer } from 'ws';

const ws = new WebSocket("ws://10.10.10.110:3000/dmx");

const app = express();
const server = app.listen(5000);

const webSocketServer = new WebSocketServer({
  noServer: true
});

server.on('upgrade', (request, socket, head) => {
  webSocketServer.handleUpgrade(request, socket, head, (websocket) => {
    webSocketServer.emit('connection', websocket, request);
  });
});

webSocketServer.on('connection', (socket, request) => {
  console.log(`Incomming connection on /websocket from ${request.socket.remoteAddress}:${request.socket.remotePort}`);

  socket.on('message', message => {
    ws.send(message);
  });
});


