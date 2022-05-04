import http from 'http';
import cors from 'cors';
import express from 'express';
import bodyParser from 'body-parser';

import { WebSocketServer } from 'ws';
import { Color, Lannooleaf, Graph, CoordMap } from 'lannooleaf_spi_controller';

const graph = new Graph();
const controller = new Lannooleaf();
var mapping;

const app = express();
const server = http.createServer(app);
const wss = new WebSocketServer({ server: server });

var jsonParser = bodyParser.json();

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


var packetStop = false;

app.post("/matrix", async (req, res) => {
  if (!packetStop) {
    packetStop = true;
    let array = req.body;

    let data = {};
    
    for (var pair of mapping) {
      if (!data[pair[1].address]) data[pair[1].address] = []
      
      let color = array[pair[0].y][pair[0].x];
      data[pair[1].address].push(new Color(color[0], color[1], color[2]));
    }
    
    for (const [key, value] of Object.entries(data)) {
      if (value.length != 16) continue;
      await controller.SetLedString(parseInt(key), value);
    }
    
    packetStop = false;
  }

  res.status(200).send();
});

/*------------------------------------- Websocket stuff -------------------------------------*/

wss.on('connection', async ws => {
  ws.on('message', async message => {
    let json_message = JSON.parse(message);

    for (var address in json_message) {
      let colorString = [];

      json_message[address].forEach(color => { colorString.push(new Color(color[0], color[1], color[2])); });
      
      await controller.SetLedString(parseInt(address), colorString);
    }
  });
});

server.listen(3000, async () => {
  await controller.Init();
  await controller.GetGraph(graph);
  let map = new CoordMap(graph);

  mapping = scaleToMatrix(map.coordToJson());
  console.log(`Server started on port ${(server.address()).port} :)`);
});

//! TODO: Place in other file
function scaleToMatrix(coord_led_array) {
  const N = 12;
  coord_led_array = JSON.parse(coord_led_array);

  let smallest_x = null;
  let smallest_y = null;

  for (var pair of coord_led_array) {
    let x = pair[0].x;
    let y = pair[0].y;

    if (smallest_x == null || x < smallest_x) smallest_x = x;
    if (smallest_y == null || y < smallest_y) smallest_y = y;
  };

  for (var pair of coord_led_array) {
    pair[0].x += Math.abs(smallest_x);
    pair[0].y += Math.abs(smallest_y);
  }

  let biggest_x = null;
  let biggest_y = null;

  for (var pair of coord_led_array) {
    let x = pair[0].x;
    let y = pair[0].y;

    if (biggest_x == null || x > biggest_x) biggest_x = x;
    if (biggest_y == null || y > biggest_y) biggest_y = y;
  };

  for (var pair of coord_led_array) {
    pair[0].x = Math.floor(pair[0].x * (N / biggest_x));
    pair[0].y = Math.floor(pair[0].y * (N / biggest_y));
  }

  return coord_led_array;
}
