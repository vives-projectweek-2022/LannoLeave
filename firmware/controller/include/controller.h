#pragma once

#include <map>
#include <vector>
#include <stdint.h>
#include <functional>

#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>

#include <i2c_fifo.h>
#include <i2c_master.h>
#include <spi_command_handler.h>

#include <graph.h>
#include <commands.h>

#include <PicoLed.hpp>

#define LED_PIN 15
#define LED_LENGTH 8

namespace LannoLeaf {

  class Controller {

    public:
      Controller(i2c_inst_t * i2c_leaf_inst);
      ~Controller();

    public:
      /** \brief Starts device discorvery algorithm*/
      void device_discovery(void);

      /** \brief Starts topology discovery algorithm*/
      void topology_discovery(void);

      /** \brief Resets all slaves and reruns discovery/topology discovery algorithm*/
      void reset(void);
      
      void handel_packet(m_commands pkt);
      
      void add_packet_handel(m_commands cmd, std::function<void(void)> func);

    private:
      void initialize(void);

    private:  
      /** \returns Next unused i2c address*/
      uint8_t get_next_available_address(void);

      /** \brief Assings new address to device if present
       * \returns Assigned address on succes, UNCONFIGUREDADDRESS on fail*/
      uint8_t assign_new_address(void);


    public:
      Graph graph;

    public:
      PicoLed::PicoLedController ledstrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LED_PIN, LED_LENGTH, PicoLed::FORMAT_GRB);

    public:
      I2CMaster leaf_master;
      Spi_command_handler *command_handler;

    private:
      std::vector<uint8_t> visited;
      std::map<m_commands, std::function<void(void)>> packet_handlers;
  
  };
  
}
