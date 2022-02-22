#pragma once

#include "mbed.h"
#include "commands.h"
#include "tiledata.h"

namespace LannooLeaf {

  class Controller {

    private:
      DigitalOut* select[4];
      I2C* i2c_bus;

    private:
      TileData controller_data;

    public:
      TileData tiles[11][11]; 

    private:
      char response_buffer[4];
    
    public:
      Controller(void);
      ~Controller();

    public:
      void update(void);

    private:
      // Send message to slave and places optional response in response buffer
      void send_message(uint8_t address, Packet packet);
      void search_neigbor(uint8_t address);

    public:
      // Scans bus for I²C devices returns address of found device
      uint8_t scan_i2c_devices(void);
  
  };

}