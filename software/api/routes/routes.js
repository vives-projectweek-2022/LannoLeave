import multer from 'multer';
import express from 'express';
import bodyParser from 'body-parser';
import child_process from 'child_process';
import { Color } from 'lannooleaf_spi_controller';

export default async (controller, graph, coorMap, __dirname) => {
  const spawn = child_process.spawn;

  const storage = multer.diskStorage({
    destination: (req, file, cb) => {
      cb(null, 'uploads/'); 
    },

    filename: (req, file, cb) => {
      cb(null, file.originalname);
    }
  });

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

  router.post('/image', (req, res) => {
    let upload = multer({ storage: storage }).single('image');

    upload(req, res, err => {

      // if (!res.file.originalname.match(/\.(PNG|png|gif|GIF)&/)) {
      //   return res.status(422).send({
      //     error: "File type can only be if gif or png type"
      //   });
      // }

      if (!req.file) {
        return res.send('Please select an image to upload');
      }

      else if (err instanceof multer.MulterError) {
        return res.send(err);
      }

      else if (err) {
        return res.send(err);
      }

      res.send(`Succesfully uploaded image`);
    });

  });

  // router.post('/python', (req, res) => {

  //   if (python_running) {
  //     return res.status(400).send({
  //       error: "Python procces already running"
  //     });
  //   }

  //   python_running = true;
  //   const python_path = path.join(__dirname, "../python_scripts/")

  //   let pythonProcces = spawn('python', 
  //   [
  //     "python_scripts/client.py",
  //     "--image", `${python_path}image/test.gif`,
  //     "--ip", "127.0.0.1"
  //   ]);    

  //   pythonProcces.stdout.on('data', data => {
  //     console.log(data.toString());
  //   });

  //   pythonProcces.stderr.on('data', data => {
  //     console.error(data.toString());
  //     return res.status(400).send({
  //       error: data.toString()
  //     });
  //   });

  //   pythonProcces.on('exit', () => {
  //     python_running = false;
  //   });
    
  //   res.status(200).send();
  // });

  return router;
};
