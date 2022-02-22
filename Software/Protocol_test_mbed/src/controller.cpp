#include "controller.h"

namespace LannooLeaf {

  void Controller::update(void) {
    // Main loop for controller
  }

  void Controller::search_neigbor(uint8_t address) {
    // TODO: Search for neighbors
  }

  void Controller::send_message(uint8_t address, Packet packet) {
    char data[4] = { packet.command, packet.optional, packet.optional_1, packet.optional_2 };
    i2c_bus->write(address, data, 4);
    i2c_bus->read(address, response_buffer, 4);
  }

  uint8_t Controller::scan_i2c_devices(void) {
    for (int address = 1; address < 127; address++) {
      int ack = i2c_bus->write(address, 0x00, 1);
      if (ack == 0) return address;
    }

    return 0;
  }

  Controller::Controller(void) {
    select[0] = new DigitalOut(D15);
    select[1] = new DigitalOut(D14);
    select[2] = new DigitalOut(D13);
    select[3] = new DigitalOut(D12);

    i2c_bus = new I2C(I2C_SDA, I2C_SCL);

    controller_data.address = 0xFF;

    tiles[5][5] = controller_data;
  }
  
  Controller::~Controller() {
    for (int i = 0; i < 4; i++) delete select[i];
    delete i2c_bus;
  }

}