import SPI from "pi-spi"

export class LannooLeaf {

  constructor() {
    this.spi = SPI.initialize("/dev/spidev0.0");
    this.spi.dataMode(SPI.mode.CPOL);
  }

  get_adjecenty_list() {
    // TODO: Implement
  }

  set_leaf_all(address, Color) {

  }

  set_unit_all(address, subaddress, Color) {
    let data = Buffer.from([0x04, address, subaddress, Color.red, Color.green, Color.blue, Color.white]);
    this.spi.write(data, (err, d) => {
      if (err) console.error(err);
      console.log(data);
    });
  }

  set_all_all(Color) {
    let data = Buffer.from([ 0x05, Color.red, Color.green, Color.blue, Color.white]);
    this.spi.write(data, (err, d) => {
      if (err) console.error(err);
      console.log(data);
    });
  }

}
