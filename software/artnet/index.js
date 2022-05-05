const axios = require('axios');
var dmxlib=require('dmxnet');

var matrixSize = 13;
var apiUrl = 'http://172.16.109.194:3000/matrix';

var dmxnet = new dmxlib.dmxnet({
  log: { level: 'info' }, // Winston logger options
  oem: 0, //OEM Code from artisticlicense, default to dmxnet OEM.
  sName: "Text", // 17 char long node description, default to "dmxnet"
  lName: "Long description", // 63 char long node description, default to "dmxnet - OpenSource ArtNet Transceiver"
  hosts: ["127.0.0.1"] // Interfaces to listen to, all by default
});
  
var receiver=dmxnet.newReceiver({
  subnet: 0, //Destination subnet, default 0
  universe: 0, //Destination universe, default 0
  net: 0, //Destination net, default 0
});
  
// This calls when an artnet package is reicieved (artnet is broadcast)
var ClearToSend = true;
receiver.on('data', async function(data) {
  // Data is an 1d array with intensity values between 0 and 255. this is for each DMX address/channel
  // The code below converts a led matrix in dmx to a 2D array with rgb colors
  
  // console.log('DMX data:', data); // this prints the full DMX aray

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

  // Code below sends the matrix to the api
  if(ClearToSend) {
    // ClearToSend = false
    axios
      .post(apiUrl, dataArray)
      .then(res => {
        // console.log(`statusCode: ${res.status}`);
        // console.log(data[0]); // this prints the first DMX channel
      })
      .catch(error => {
        // console.error(error);
        console.log(`ERROR`);
      }); 
      await new Promise(resolve => {setTimeout(resolve, 40)})
      ClearToSend = true;
  }

});




