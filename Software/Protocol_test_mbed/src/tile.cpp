#define SLAVE_ADDRESS 0x01
#include "tile.h"

namespace LannooLeaf {

  void Tile::update(void) {
    update_select_state();

    if (select_state != 0x00) {
      int operation = bus->receive();

      switch (operation) {
        // TODO: Handel commands
        // Unicast
        case I2CSlave::ReadAddressed:
          break;

        case I2CSlave::WriteAddressed:
          break;

        // Broadcast
        case I2CSlave::WriteGeneral:
          break;
      }
    }
  }

  void Tile::update_select_state(void) {
    select_state = 0x00;
    
    for (uint8_t i = 0; i < 4; i++) { 
      select_state |= select[i]->read() << i;
    }
  }

  Tile::Tile(void) {
    select[0] = new DigitalInOut(D15);
    select[1] = new DigitalInOut(D14);
    select[2] = new DigitalInOut(D13);
    select[3] = new DigitalInOut(D12);

    bus = new I2CSlave(I2C_SDA, I2C_SCL);
    bus->address(SLAVE_ADDRESS);
  }
  
  Tile::~Tile() {
    for (int i = 0; i < 4; i++) delete select[i];
    delete bus;
  }

}