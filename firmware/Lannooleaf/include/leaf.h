#pragma once

#include <map>
#include <stdio.h>
#include <stdint.h>
#include <functional>

#include <PicoLed.hpp>

#include <commands.h>
#include <i2c_slave.h>
#include <command_handler.h>
#include <helper_funcs_var.h>

#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>

namespace Lannooleaf {

  class Leaf {

    public:
      Leaf();
      ~Leaf();
  
    public:
      /** \brief Updates the select pin status and handels received commands */
      void add_leaf_handlers(CommandHandler* handler);

    public:
      /** \returns Slave select pin status as uint8_t */
      uint8_t sel_pin_status(void) { return _sel_pin_status; }

      void update(void) { update_sel_status(); }


    public:
      PicoLed::PicoLedController ledstrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LED_PIN, LED_LENGTH, PicoLed::FORMAT_GRB);

    private:
      void update_sel_status(void);

    private:
      uint8_t _sel_pin_status;

    public:
      std::vector<std::pair<uint8_t, uint8_t>> neighbors;
 
  };

}
