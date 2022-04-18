import spi from 'spi-device';
import { Gpio } from 'onoff';

const Commands = {
  helloMessage: 0x01,
  getAdjListSize: 0x02,
  getAdjList: 0x03,
  setLed: 0x04,
  setAllLeds: 0x05,
  setLedString: 0x06
}

export default class Lannooleaf {
  constructor() {
    this.spi_controller = null;
    this.cs = new Gpio(25, 'out');
  }

  WaitForDataBegin() {
    return new Promise(async resolve => {
      let byte;
      await this.getOneByte().then(data => {
        byte = data;
      });

      while (byte != 0x00) {
        await this.getOneByte().then(data => {
          byte = data;
        });
      };

      resolve();
    });
  }

  helloMessage() {
    return new Promise(async resolve => {

      let helloMessage = [{
        byteLength: 1,
        sendBuffer: Buffer.from([0x01]),
      }];
      
      this.cs.write(0);
      this.spi_controller.transfer(helloMessage, async (error) => {
        if (error) throw error;
        this.cs.write(1);

        await this.WaitForDataBegin();

        let message = "";
        for (var i = 0; i < 10; i++) await this.getOneByte().then(data => {
          message = message.concat(String.fromCharCode(data));
        });

        resolve(message);
      });

    });
  }

  // Should be private
  GetAdjListSize() {
    return new Promise(async resolve => {
      let message = [{
        byteLength: 1,
        sendBuffer: Buffer.from([Commands.getAdjListSize])
      }];
  
      this.cs.write(0);
      this.spi_controller.transfer(message, async error => {
        if (error) throw error;
        this.cs.write(1);

        await this.WaitForDataBegin();

        let msb;
        await this.getOneByte().then(byte => {
          msb = byte;
        });

        let lsb;
        await this.getOneByte().then(byte => {
          lsb = byte;
        });

        let count = ((msb & 0xff) << 8 | (lsb & 0xff));
        resolve(count);
      })
    });
  }

  getAdjList(graph) {
    return new Promise(async resolve => {
      await this.GetAdjListSize().then(size => {
        let message = [{
          byteLength: 1,
          sendBuffer: Buffer.from([Commands.getAdjList])
        }];

        this.cs.write(0);
        this.spi_controller.transfer(message, async error => {
          if (error) throw error;
          this.cs.write(1);

          await this.WaitForDataBegin();

          let data = [];
          for (var i = 0; i < size; i++) {
            await this.getOneByte().then(byte => {
              data.push(byte);
            });
          }

          var i = 0;
          while (data[i] != 0) {
            graph.AddNode(data[i]);
            i++;
          }

          i++;

          for (i; i < data.length; i+=3) {
            graph.AddEdge(data[i], data[i+1], data[i+2]);
          }

          resolve(data);
        });
      });
    });
  }

  setAll(color) {
    return new Promise(resolve => {
      let color_message = [{
        byteLength: 4,
        sendBuffer: Buffer.from([Commands.setAllLeds, color.red, color.green, color.blue]),
      }];

      this.cs.write(0);
      this.spi_controller.transfer(color_message, (error, message) => {
        if (error) throw error;
        this.cs.write(1);
        resolve(message);
      });
    });
  }

  setLed(address, led, color) {
    return new Promise(resolve => {
      let set_led_message = [{
        byteLength: 6,
        sendBuffer: Buffer.from([Commands.setLed, address, led, color.red, color.green, color.blue])
      }];

      this.cs.write(0);
      this.spi_controller.transfer(set_led_message, (error, message) => {
        if (error) throw error;
        this.cs.write(1);
        resolve(message);
      });
    });
  }

  getOneByte() {
    let getOneByte = [{
      byteLength: 1,
      sendBuffer: Buffer.from([0xa5]),
      receiveBuffer: Buffer.alloc(1)
    }];
    
    this.cs.write(0);
    return new Promise(resolve => {

      this.spi_controller.transfer(getOneByte, (error, message) => {
        if (error) throw error;
        this.cs.write(1);
        resolve(message[0].receiveBuffer[0]);
      });

    });
  }

  setLedString(address, colorString) {
    let message = [{
      byteLength: 50,
      sendBuffer: Buffer.alloc(50, 0x00),
    }];

    message[0].sendBuffer[0] = Commands.setLedString;
    message[0].sendBuffer[1] = address;

    var index = 2;
    colorString.forEach(color => {
      message[0].sendBuffer[index    ] = color.red;
      message[0].sendBuffer[index + 1] = color.green;
      message[0].sendBuffer[index + 2] = color.blue;

      index = index + 3;
    });

    this.cs.write(0);
    return new Promise(resolve => {
      this.spi_controller.transfer(message, (error, message) => {
        if (error) throw error;
        this.cs.write(1);
        resolve(message);
      });
    });
  }

  wake() {
    this.cs.writeSync(1);
    this.cs.writeSync(0);
    this.cs.writeSync(1);
  
    return new Promise(resolve => { setTimeout(resolve, 500)});
  }

  Init() {
    return new Promise(async resolve => {
      this.spi_controller = spi.open(0, 0, error => {
        if (error) throw error;
        this.spi_controller.setOptions({
          mode: spi.MODE3,
          maxSpeedHz: 1000000,
          noChipSelect: true
        }, error => {
          if (error) throw error;
          resolve();
        });
      });
    });
  }

  Destroy(cb) {
    if (!this.spi_controller) {
      cb("Destroying Lannooleaf when it was never initialized");
    } else {
      this.cs.unexport();
      this.spi_controller.close(error => {
        if (error) throw error;
      });
    }
  }

}
