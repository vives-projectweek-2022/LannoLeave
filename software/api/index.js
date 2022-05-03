import http from 'http';
import cors from 'cors';
import express from 'express';
import bodyParser from 'body-parser';

import { WebSocketServer } from 'ws';
import { Color, Lannooleaf, Graph, CoordMap } from 'lannooleaf_spi_controller';

const graph = new Graph();
const controller = new Lannooleaf();

const app = express();
const server = http.createServer(app);
const wss = new WebSocketServer({ server: server });

var run = true;

app.use(cors());
app.use(bodyParser.json());

/*------------------------------------- GetNPost stuff -------------------------------------*/

app.get('/', (req, res) => {
  res.send('Welcome to API for Lannoo leaf');
});

app.get("/count", (req, res) => {
  res.send({
    size: graph.map.size()
  });
});

app.get("/coordinates", async (req, res) => {
  const coorMap = new CoordMap(graph);
  res.send(coorMap.coordToJson());
});

app.post('/', jsonParser, async (req, res) => {
  console.log(JSON.stringify(req.body));
  
  if(req.body.color) {
    let color = req.body.color.substring(1);

    let red = Number('0x' + color.substring(0,2));
    let green = Number('0x' + color.substring(2,4));
    let blue = Number('0x' + color.substring(4));

    await controller.SetAll(new Color(red, green, blue));
  }

  res.send('Your data: ' + JSON.stringify(req.body));
});

/*------------------------------------- Websocket stuff -------------------------------------*/

wss.on('connection', async ws => {
  ws.on('message', async message => {
    let json_message = JSON.parse(message);

    for (var address in json_message) {
      let colorString = [];

      json_message[address].forEach(color => { colorString.push(new Color(color[0], color[1], color[2])); });
      
      await controller.SetLedString(parseInt(address), colorString);
      
      ws.send(run);
    }
  });
});

server.listen(3001, async () => {
  await controller.Init();
  await controller.GetGraph(graph);
  console.log(`Server started on port ${(server.address()).port} :)`);
});
