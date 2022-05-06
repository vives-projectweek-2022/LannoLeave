import express from 'express';
import bodyParser from 'body-parser';
import { Color } from 'lannooleaf_spi_controller';

export default async (controller, graph, coorMap) => {
  const router     = express.Router();
  const jsonParser = bodyParser.json();

  /*------------------------------------ GET ------------------------------------*/
  
  router.get('/', (req, res) => {
    res.send('Welcome to API for Lannoo leaf');
  });
  
  router.get("/count", (req, res) => {
    res.send({
      size: graph.map.size()
    });
  });
  
  router.get("/coordinates", async (req, res) => {
    res.send(coorMap.coordToJson());
  });
  
  /*------------------------------------ POST ------------------------------------*/
  
  router.post('/', jsonParser, async (req, res) => {
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

  return router;
};
