const spi = require('spi-device');

const options = {
  mode: spi.MODE3,
  maxSpeedHz: 4000000,
};

const frequency = 4000000;
const lannoLeaf = spi.openSync(0, 0, options);

console.log(lannoLeaf.getOptionsSync());

// https://www.npmjs.com/package/spi-device#opensyncbusnumber-devicenumber-options

function get_matrix_size() {
  let data = [{
    sendBuffer: Buffer.from([0x01, 0xff, 0xff]),
    receiveBuffer: Buffer.alloc(3),
    byteLength: 3,
    speedHz: frequency,
  }];

  lannoLeaf.transfer(data, (err, message) => {
    if (err) throw err;
    console.log(message);
  });
}

function set_all_color(r, g, b) {
  let data = [{
    sendBuffer: Buffer.from([0x03, r, g, b]),
    receiveBuffer: Buffer.alloc(4),
    byteLength: 4,
  }];

  lannoLeaf.transfer(data, (err, message) => {
    if (err) throw err;
    console.log(message);
  });
}

async function main() {

  get_matrix_size();

  // set_all_color(0xff, 0x00, 0x00);
  // await new Promise(r => {setTimeout(r, 1000)});

  // set_all_color(0x00, 0xff, 0x00);
  // await new Promise(r => {setTimeout(r, 1000)});

  // set_all_color(0x00, 0x00, 0xff);
  // await new Promise(r => {setTimeout(r, 1000)});

  lannoLeaf.closeSync();
}

main();
