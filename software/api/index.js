import path from 'path';
import cors from 'cors';
import express from 'express';
import bodyParser from 'body-parser';
import routes from './routes/routes.js';
import websocket from './websocket/websocket.js';

import { fileURLToPath } from 'url'; 

import { scaleToMatrix } from './middleware/functions.js';
import { Lannooleaf, Graph, CoordMap } from 'lannooleaf_spi_controller';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const app = express();

const controller = new Lannooleaf();
await controller.Init();

const graph      = await controller.GetGraph();
const coorMap    = new CoordMap(graph);
const mapping    = scaleToMatrix(coorMap.coordToJson());

app.use(cors());
app.use(bodyParser.json());
app.use(express.static(__dirname + '/public'));
app.use(await routes(controller, graph, coorMap, __dirname));

const server = app.listen(3000, () => {
  console.log(`Server started listening on port: ${3000}\n\n`);
});

websocket(server, controller, mapping);
