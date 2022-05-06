import * as dmxlib from 'dmxnet';
import WebSocket from 'ws';

const HOST = "10.10.10.110";
const PORT = 3000;

var matrixSize = 13;
var apiUrl = `ws://${HOST}:${PORT}/dmx`;

const websocket = new WebSocket(apiUrl);

var dmxnet = new dmxlib.dmxnet({
  log: { level: 'info' }, // Winston logger options
  oem: 0, //OEM Code from artisticlicense, default to dmxnet OEM.
  sName: "Text", // 17 char long node description, default to "dmxnet"
  lName: "Long description", // 63 char long node description, default to "dmxnet - OpenSource ArtNet Transceiver"
  hosts: ["127.0.0.1"] // Interfaces to listen to, all by default
});

var receiver = dmxnet.newReceiver({
  subnet: 0, //Destination subnet, default 0
  universe: 0, //Destination universe, default 0
  net: 0, //Destination net, default 0
});

// This calls when an artnet package is reicieved (artnet is broadcast)
receiver.on('data', async function(data) {
  // Data is an 1d array with intensity values between 0 and 255. this is for each DMX address/channel
  // The code below converts a led matrix in dmx to a 2D array with rgb colors
  
  console.log('DMX data:', data[0]); // this prints the full DMX aray

  let dataArray = new Array(matrixSize);
  let addressCounter = 0;
  for (let i = 0; i < matrixSize; i++) {
    dataArray[i] = new Array(matrixSize);
    for (let ii = 0; ii < matrixSize; ii++) {
      // for snake conversion
      if (i%2 == 1) {
        dataArray[i][matrixSize-1-ii] = [data[addressCounter++],data[addressCounter++],data[addressCounter++]];
      }
      else {
        dataArray[i][ii] = [data[addressCounter++],data[addressCounter++],data[addressCounter++]];
      }
    }
  }

  websocket.send(JSON.stringify(dataArray), err => {
    if (err) console.error(err);
  });

});
