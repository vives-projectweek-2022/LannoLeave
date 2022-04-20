import express from 'express';
import { Lannooleaf, Color, Graph } from 'lannooleaf_spi_controller';
import bodyParser from 'body-parser';
import cors from 'cors';
const controller = new Lannooleaf();
controller.Init()
.then(() => {
  const app = express();
  const port = 3000;


  app.use(cors());
  app.use(bodyParser.json());
  var jsonParser = bodyParser.json();

  app.get('/', (req, res) => {
    res.send('Welcome to API for Lannoo leaf');
  })

  app.post('/', jsonParser, (req, res) => {
    console.log(JSON.stringify(req.body));
    res.send('Your data: ' + JSON.stringify(req.body));
    if(req.body.effect) {
      effect1();
    }
    if(req.body.color) {
      let color = req.body.color.substring(1);
      let red = Number('0x' + color.substring(0,2));
      let green = Number('0x' + color.substring(2,4));
      let blue = Number('0x' + color.substring(4));
      controller.SetAll(new Color(red,green,blue));
    }
  })

  app.listen(port, () => {
    console.log(`Example app listening on port ${port}`);
  })

});

async function effect1 () {
  for (let index = 0; index < 20; index++) {
    for (let i = 0; i < 255; i++) {
      await controller.SetAll(new Color(i,0,0));
      await new Promise(resolve => {setTimeout(resolve,10)})
    }
    for (let i = 0; i < 255; i++) {
      await controller.SetAll(new Color(0,i,0));
      await new Promise(resolve => {setTimeout(resolve,10)})
    }
    for (let i = 0; i < 255; i++) {
      await controller.SetAll(new Color(0,0,i));
      await new Promise(resolve => {setTimeout(resolve,10)})
    }
  }
}
