import express, { json } from 'express';
import http from 'http';
import { WebSocketServer } from 'ws';
import { Color, Lannooleaf, Graph, CoordMap } from 'lannooleaf_spi_controller';

const controller = new Lannooleaf();
const graph = new Graph();

const app = express();
const server = http.createServer(app);
const wss = new WebSocketServer({ server: server });

wss.on('connection', async ws => {

  // Listening on websocket for message
  ws.on('message', async message => {
    let json_message = JSON.parse(message);

    for (var address in json_message) {
      let colorString = [];

      // Contructing led string from received json data
      json_message[address].forEach(color => {
        colorString.push(new Color(color[0], color[1], color[2]));
      });
      
      // Sending ledstring to lannooleaf on error just log
      try {
        await controller.SetLedString(parseInt(address), colorString);
      } catch (error) {
        console.error(error)
      }
    }

  });

});

// Get request for python script to get coordinates
app.get("/coordinates", async (req, res) => {
  const coorMap = new CoordMap(graph);
  res.send(coorMap.coordToJson());
});

// start our server
server.listen(3001, async () => {
  await controller.Init();
  // Getting graph from lannooleaf
  await controller.GetGraph(graph);
  console.log(`Server started on port ${(server.address()).port} :)`);
});
