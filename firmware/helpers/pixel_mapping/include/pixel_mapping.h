#pragma once

#include <map>
#include <vector>

#include <graph.h>

namespace Lannooleaf {

  struct Pixel {
    uint8_t i2c_address = UNCONFIGUREDADDRESS;
    std::pair<uint, uint> led = std::make_pair(0, 0);

    Pixel& operator= (Pixel a) {
      i2c_address = a.i2c_address;
      led = a.led;

      return *this;
    }

    std::string to_string(void) {
      return "i2c_address: " + std::to_string(i2c_address) + " leds: [" + std::to_string(led.first) + ", " + std::to_string(led.second) + "]";
    }
  };
  

  class PixelMapping {

    public:
      PixelMapping(Graph* graph);
      ~PixelMapping();

    public:
      /** \brief Start generation of pixel_mapping*/
      void generate_mapping(void);

    public:
      /** \brief Get pixel info by using index 
       * \returns Pixel stuct of pixel at index, empty pixel stuct if no exitst.*/
      Pixel get_pixel(uint number);

    public:
      /** \returns std::pair of matrix size */
      std::pair<int, int> get_matrix_size(void) { return std::make_pair(x_size, y_size); }

    private:
      Graph* graph;
      std::map<uint, Pixel> pixel_mapping;

    private:
      int x_size;
      int y_size;

  };

}
