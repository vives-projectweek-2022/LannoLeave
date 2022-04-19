#pragma once

#include <map>
#include <iostream>
#include <stdint.h>
#include <functional>

namespace Lannooleaf {

  class CommandHandler {

    public:
      CommandHandler();
      ~CommandHandler();

    public:
      /** \brief Add a handler for a command (uint8_t) that can then be executed later by referencing the comman */
      void add_handler(uint8_t cmd, std::function<void(void)> handler);

      /** \brief Execute a handler for a command, if no handler is found will just return*/ 
      void handel_command(uint8_t cmd);

    private:
      std::unordered_map<uint8_t, std::function<void(void)>> handler_map;

  };

}
