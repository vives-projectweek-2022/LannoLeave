import WebSocket from 'ws';
import express from 'express';
import { WebsocketServer } from 'ws';

const ws = new WebSocket("ws://10.10.10.110:3000/dmx");

const app = express();

const webSocketServer = new WebsocketServer({
  noServer: true,
  path: '/'
});

app.on('upgrade', (request, socket, head) => {
  webSocketServer.handleUpgrade(request, socket, head, (websocket) => {
    webSocketServer.emit('connection', websocket, request);
  });
});

webSocketServer.on('connection', (socket, request) => {
  socket.on('message', message => {
    ws.send(message);
  });
});

app.listen(5000);
