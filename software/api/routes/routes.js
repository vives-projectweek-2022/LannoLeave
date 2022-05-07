import path from 'path';
import express from 'express';
import { fileURLToPath } from 'url';
import bodyParser from 'body-parser';
import child_process from 'child_process';
import { Color } from 'lannooleaf_spi_controller';

export default async (controller, graph, coorMap) => {
  const router     = express.Router();
  const jsonParser = bodyParser.json();

  const spawn = child_process.spawn;
  var python_running = false;

  const __filename = fileURLToPath(import.meta.url);
  const __dirname = path.dirname(__filename);

  await controller.SetAll(new Color(0, 75, 0));

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

  router.post('/python', (req, res) => {

    if (python_running) {
      return res.status(400).send({
        error: "Python procces already running"
      });
    }

    python_running = true;
    const python_path = path.join(__dirname, "../python_scripts/")

    let pythonProcces = spawn('python', 
    [
      "python_scripts/client.py",
      "--image", `${python_path}image/test.gif`,
      "--ip", "127.0.0.1"
    ]);    

    pythonProcces.stdout.on('data', data => {
      console.log(data.toString());
    });

    pythonProcces.stderr.on('data', data => {
      console.error(data.toString());
      return res.status(400).send({
        error: data.toString()
      });
    });

    pythonProcces.on('exit', () => {
      python_running = false;
    });
    
    res.status(200).send();
  });

  return router;
};
