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
          dataArray[i][ii] = [data[addressCounter++],data[addressCounter++],data[addressCounter++]];
        }
        else {
          dataArray[i][12-ii] = [data[addressCounter++],data[addressCounter++],data[addressCounter++]];
        }
      }
    }
    // console.log(dataArray);
    if(ClearToSend) {
      ClearToSend = false
      axios
        .post('http://10.5.8.2:3000/matrix', dataArray)
        .then(res => {
          console.log(`statusCode: ${res.status}`);
        })
        .catch(error => {
          // console.error(error);
          console.log(`ERROR`);
        }); 
        await new Promise(resolve => {setTimeout(resolve, 40)})
        ClearToSend = true;
    }
    // console.log(data[0]);
    // console.log('DMX data:', data);
  });




