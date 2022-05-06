import { WebSocketServer } from "ws";
import { Color } from "lannooleaf_spi_controller";

export default async (expressServer, controller, mapping) => {

  const webSocketServer = new WebSocketServer({
    noServer: true
  });

  const dmxSocketServer = new WebSocketServer({
    noServer: true
  });

  expressServer.on('upgrade', (request, socket, head) => {
    const path = request.url;

    if (path ==='/websocket') {
      webSocketServer.handleUpgrade(request, socket, head, (websocket) => {
        webSocketServer.emit('connection', websocket, request);
      });
    }

    if (path === '/dmx') {
      dmxSocketServer.handleUpgrade(request, socket, head, (websocket) => {
        dmxSocketServer.emit('connection', websocket, request);
      });
    }

  });

  webSocketServer.on('connection', (socket, request) => {

    console.log(`Incomming connection on /websocket from ${request.socket.remoteAddress}:${request.socket.remotePort}`);

    socket.on('message', async message => {
      let json = JSON.parse(message);

      for (var address in json) {
        let colorString = [];
        
        json[address].forEach(color => colorString.push(new Color(color[0], color[1], color[2])));
        
        await controller.SetLedString(parseInt(address), colorString);
      }
    });
    
  });

  dmxSocketServer.on('connection', (socket, request) => {
    
    console.log(`Incomming connection on /dmx from ${request.socket.remoteAddress}:${request.socket.remotePort}`);

    socket.on('message', async message => {
      let array = JSON.parse(message.toString());
      let data = {};
    

      for (var pair of mapping) {
        if (!data[pair[1].address]) data[pair[1].address] = []

        let color = array[pair[0].x][pair[0].y];
        
        data[pair[1].address].push(new Color(color[0], color[1], color[2]));
      }

      for (const [key, value] of Object.entries(data)) {
        if (value.length != 16) continue;
        await controller.SetLedString(parseInt(key), value);
      }
    });

  });

};
