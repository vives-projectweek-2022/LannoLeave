#include "spi_slave.h"

namespace Lannooleaf {

  void Spi_slave::initialize(uint mosi, uint miso, uint clk, uint cs) {
    static bool initialized = false;
    
    if (!initialized) {
      gpio_init(cs);

      gpio_set_function(mosi, GPIO_FUNC_SPI);
      gpio_set_function(miso, GPIO_FUNC_SPI);
      gpio_set_function(clk, GPIO_FUNC_SPI);
      gpio_set_function(cs, GPIO_FUNC_SPI);

      reset_block(RESETS_RESET_SPI0_BITS);
      unreset_block_wait(RESETS_RESET_SPI0_BITS);  

      spi_set_baudrate(spi0, 1000000);
    
      hw_set_bits(&spi_get_hw(spi0)->dmacr, SPI_SSPDMACR_TXDMAE_BITS | SPI_SSPDMACR_RXDMAE_BITS);

      spi_set_slave(spi0, true);

      // 8 bit transfers / SPI Mode3 / MSB first
      spi_set_format(spi0, 
      8,
      SPI_CPOL_1,
      SPI_CPHA_1,
      SPI_MSB_FIRST);

      hw_set_bits(&spi_get_hw(spi0)->imsc, SPI_SSPIMSC_TXIM_BITS);
      irq_set_exclusive_handler(SPI0_IRQ, spi_irq_handler);
      irq_set_enabled(SPI0_IRQ, true);

      irq_set_priority(SPI0_IRQ, PICO_DEFAULT_IRQ_PRIORITY - 10);

      gpio_set_irq_enabled_with_callback(cs, GPIO_IRQ_EDGE_RISE, true, cs_callback);

      // Enable spi
      hw_set_bits(&spi_get_hw(spi0)->cr1, SPI_SSPCR1_SSE_BITS);

      initialized = true;
    }
  }

  void __not_in_flash_func(Spi_slave::cs_callback)(uint gpio, uint32_t events) {
    while (spi_get_hw(spi0)->sr & 0x04) {
      uint8_t value = spi_get_hw(spi0)->dr;
      Spi_slave::Get()._read_fifo.push(value);
    }
  }

  void __not_in_flash_func(Spi_slave::spi_irq_handler)(void) {
    uint32_t status = spi_get_hw(spi0)->mis;

    // Tx fifo half empty or less
    while (status & SPI_SSPMIS_TXMIS_BITS) {
      // Adding 0xff if writefifo is empty
      spi_get_hw(spi0)->dr = Spi_slave::Get()._write_fifo.empty() ? 0xff : Spi_slave::Get()._write_fifo.front();
      if (!Spi_slave::Get()._write_fifo.empty()) Spi_slave::Get()._write_fifo.pop();

      status = spi_get_hw(spi0)->mis;
    }

    while (spi_get_hw(spi0)->sr & 0x04) {
      uint8_t value = spi_get_hw(spi0)->dr;
      Spi_slave::Get()._read_fifo.push(value);
    }
  }

}
