#include <spi.hpp>

namespace Lannooleaf::baseclasses {

  SPI::SPI(spi_inst_t* spi, uint mosi, uint miso, uint clk, uint cs, uint frequency)
  : mosi(mosi), miso(miso), clk(clk), cs(cs) {
    assert(spi == spi0 || spi == spi1);

    static bool spi0_init = false;
    static bool spi1_init = false;

    if (spi == spi0 && spi0_init || spi == spi1 && spi1_init) {
      throw std::runtime_error("ERROR: tried to create a new instance of already in use spi hardware");
    }

    if (spi == spi0) {
      spi0_init = true;
    } else {
      spi1_init = true;
    }

    gpio_init(cs);

    gpio_set_function(mosi, GPIO_FUNC_SPI);
    gpio_set_function(miso, GPIO_FUNC_SPI);
    gpio_set_function(clk, GPIO_FUNC_SPI);
    gpio_set_function(cs, GPIO_FUNC_SPI);

    spi_init(spi, frequency);

    spi_set_format(spi, 
      8,
      SPI_CPOL_1,
      SPI_CPHA_1,
      SPI_MSB_FIRST);
  
  }

  SPI::~SPI() {
    spi_deinit(spi);

    gpio_init(mosi);
    gpio_init(miso);
    gpio_init(clk);
    gpio_init(cs);
  }

}
