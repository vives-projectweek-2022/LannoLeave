#pragma once

#include <stdio.h>
#include <stdint.h>
#include <map>
#include <functional>

#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include <i2c_slave.h>
#include <i2c_fifo.h>

#include <commands.h>
#include <helper_funcs_var.h>
#include <command_handler.h>

#include <PicoLed.hpp>

namespace LannoLeaf {

  static read_memory l_read_mem; 
  static write_memory l_write_mem;

  static context l_context = {
    &l_read_mem,
    &l_write_mem
  };

  class Leaf {

    public:
      Leaf(uint8_t address, i2c_inst_t * i2c);
      ~Leaf();
  
    public:
      /** \brief Updates the select pin status and handels received commands */
      void update(void);
      
    public:
      /** \brief Initializes slave and start listening on the i²c bus */
      void slave_init(void);

      /** \brief Assing a new i²c address to slave 
      * \param address The new i²c slave address  */
      void address(uint8_t address);

      /** \brief Resets slave to default configuration*/
      void reset(void);

    public:
      /** \returns Slave i²c address */
      uint8_t& address(void) { return _address; }

      /** \returns Slave select pin status as uint8_t */
      uint8_t& sel_pin_status(void) { return _sel_pin_status; }

    public:
      /** \returns true if slave is initialized, fase is not*/
      bool& slave_initialized(void) { return _slave_initialized; }

      /** \returns true if slave has been configured with a new i²c address, fase if not */
      bool& configured(void) { return _configured; }

    public:
      CommandHandler l_command_handler;
      PicoLed::PicoLedController ledstrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LED_PIN, LED_LENGTH, PicoLed::FORMAT_GRB);

    private:
      void initialize(void);
      void update_sel_status(void);

    private:
      uint8_t _sel_pin_status = 0x00;

    private:
      bool _slave_initialized = false;
      bool _configured = false;

    private:
      i2c_inst_t * i2c;
      uint8_t _address;
  };

  static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event);

}
