#include "tiledata.h"

namespace LannooLeaf {

  string TileData::to_string(void) {
    string return_string;
    return_string += std::to_string(address) + " ";
    return_string += led_status ? "true" : "false";
    return_string += " " + std::to_string(Red);
    return_string += " " + std::to_string(Green);
    return_string += " " + std::to_string(Blue);
    return return_string;
  }

}