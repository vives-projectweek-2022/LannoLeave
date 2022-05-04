const axios = require('axios');
var dmxlib=require('dmxnet');

var dmxnet = new dmxlib.dmxnet({
    log: { level: 'info' }, // Winston logger options
    oem: 0, //OEM Code from artisticlicense, default to dmxnet OEM.
    sName: "Text", // 17 char long node description, default to "dmxnet"
    lName: "Long description", // 63 char long node description, default to "dmxnet - OpenSource ArtNet Transceiver"
    hosts: ["127.0.0.1"] // Interfaces to listen to, all by default
  });
  
var ClearToSend = true;

var receiver=dmxnet.newReceiver({
    subnet: 0, //Destination subnet, default 0
    universe: 0, //Destination universe, default 0
    net: 0, //Destination net, default 0
  });

  receiver.on('data', async function(data) {
    let dataArray = new Array(13);
    let addressCounter = 0;
    for (let i = 0; i < 13; i++) {
      dataArray[i] = new Array(13);
      for (let ii = 0; ii < 13; ii++) {
        if (i%2 == 1) {
          dataArray[i][12-ii] = [data[addressCounter++],data[addressCounter++],data[addressCounter++]];
        }
        else {
          dataArray[i][ii] = [data[addressCounter++],data[addressCounter++],data[addressCounter++]];
        }
      }
    }
    // console.log(dataArray);
    if(ClearToSend) {
      // ClearToSend = false
      axios
        .post('http://172.16.109.185:3000/matrix', dataArray)
        .then(res => {
          // console.log(`statusCode: ${res.status}`);
          console.log(data[0]);
        })
        .catch(error => {
          // console.error(error);
          console.log(`ERROR`);
        }); 
        await new Promise(resolve => {setTimeout(resolve, 40)})
        ClearToSend = true;
    }
    // console.log('DMX data:', data);
  });




