#include "spi_slave.h"

namespace Lannooleaf {

  Spi_slave& Spi_slave::Get(void) {
    static Spi_slave instance;
    return instance;
  }

  void Spi_slave::initialize(uint mosi, uint miso, uint clk, uint cs) {
    static bool initialized = false;
    
    if (!initialized) {
      spi_init(spi0, 1000 * 1000);
      spi_set_slave(spi0, true);

      gpio_set_function(mosi, GPIO_FUNC_SPI);
      gpio_set_function(miso, GPIO_FUNC_SPI);
      gpio_set_function(clk, GPIO_FUNC_SPI);
      gpio_set_function(cs, GPIO_FUNC_SPI);

      spi_set_format(spi0,
      8,
      SPI_CPOL_1,
      SPI_CPHA_1,
      SPI_MSB_FIRST);

      gpio_set_irq_enabled_with_callback(cs, GPIO_IRQ_EDGE_FALL, true, &cs_callback);

      initialized = true;
    }
  }

  void Spi_slave::add_write_data(uint8_t data[], size_t len) {
    assert(len <= 8);

    for (size_t i = 0; i < len; i++) {
      Spi_slave::Get().write_buffer[i] = data[i];
    }
  }

  void Spi_slave::cs_callback(uint ce, uint32_t events) {
    uint8_t len;
    spi_read_blocking(spi0, 0xff, &len, 1);
    spi_write_read_blocking(spi0, Spi_slave::Get().write_buffer, Spi_slave::Get().read_buffer, len);
    
    // Push packet on fifo 
    Spi_slave::Get().fifo.push({
      {
        Spi_slave::Get().read_buffer[0],
        Spi_slave::Get().read_buffer[1],
        Spi_slave::Get().read_buffer[2],
        Spi_slave::Get().read_buffer[3],
        Spi_slave::Get().read_buffer[4],
        Spi_slave::Get().read_buffer[5],
        Spi_slave::Get().read_buffer[6],
        Spi_slave::Get().read_buffer[7]
      }, {
        Spi_slave::Get().write_buffer[0],
        Spi_slave::Get().write_buffer[1],
        Spi_slave::Get().write_buffer[2],
        Spi_slave::Get().write_buffer[3],
        Spi_slave::Get().write_buffer[4],
        Spi_slave::Get().write_buffer[5],
        Spi_slave::Get().write_buffer[6],
        Spi_slave::Get().write_buffer[7]
      }
    });
  }

}
