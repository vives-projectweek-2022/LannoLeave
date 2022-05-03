var dmxlib=require('dmxnet');

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

  receiver.on('data', function(data) {
    console.log('DMX data:', data);
  });