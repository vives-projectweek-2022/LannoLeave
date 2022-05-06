import cors from 'cors';
import express from 'express';
import bodyParser from 'body-parser';
import routes from './routes/routes.js';
import websocket from './websocket/websocket.js';
import { scaleToMatrix } from './middleware/functions.js';
import { Lannooleaf, Graph, CoordMap } from 'lannooleaf_spi_controller';

const app = express();

const graph      = new Graph();
const controller = new Lannooleaf();

await controller.Init();
await controller.GetGraph(graph);

const coorMap    = new CoordMap(graph);
const mapping    = scaleToMatrix(coorMap.coordToJson());

app.use(cors());
app.use(bodyParser.json());
app.use(await routes(controller, graph, coorMap));

const server = app.listen(3000, () => {
  console.log(`Server started listening on port: ${3000}\n\n`);
});

websocket(server, controller, mapping);
