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
      void add_handler(uint8_t cmd, std::function<void(void)> handler);
      void handel_command(uint8_t cmd);

    private:
      std::unordered_map<uint8_t, std::function<void(void)>> handler_map;

  };

}
