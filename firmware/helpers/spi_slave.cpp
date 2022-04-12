#include <spi_slave.h>

namespace Lannooleaf {

  void Spi_slave::initialize(uint mosi, uint miso, uint clk, uint cs) {
    static bool initialized = false;
    
    if (!initialized) {
      gpio_set_function(mosi, GPIO_FUNC_SPI);
      gpio_set_function(miso, GPIO_FUNC_SPI);
      gpio_set_function(clk, GPIO_FUNC_SPI);
      gpio_set_function(cs, GPIO_FUNC_SPI);

      reset_block(RESETS_RESET_SPI0_BITS);
      unreset_block_wait(RESETS_RESET_SPI0_BITS);  

      spi_set_baudrate(spi0, 2000000);

      spi_set_format(spi0, 
                    8, 
                    SPI_CPOL_0, 
                    SPI_CPHA_0, 
                    SPI_MSB_FIRST);
    
      hw_set_bits(&spi_get_hw(spi0)->dmacr, SPI_SSPDMACR_TXDMAE_BITS | SPI_SSPDMACR_RXDMAE_BITS);

      spi_set_slave(spi0, true);
      spi_set_format(spi0, 
      8,
      SPI_CPOL_1,
      SPI_CPHA_1,
      SPI_MSB_FIRST);

      hw_set_bits(&spi_get_hw(spi0)->imsc, SPI_SSPIMSC_TXIM_BITS | SPI_SSPIMSC_RXIM_BITS);
      irq_set_exclusive_handler(SPI0_IRQ, spi_irq_handler);
      irq_set_enabled(SPI0_IRQ, true);

      // Enable spi
      hw_set_bits(&spi_get_hw(spi0)->cr1, SPI_SSPCR1_SSE_BITS);

      initialized = true;
    }
  }

  void __not_in_flash_func(Spi_slave::spi_irq_handler)(void) {
    
    uint32_t status = spi_get_hw(spi0)->mis;

    while (status & SPI_SSPMIS_RXMIS_BITS || status & SPI_SSPMIS_TXMIS_BITS) {

      // Rx fifo is half full or less
      if (status & SPI_SSPMIS_RXMIS_BITS) {
        // Push all to read_fifo until rx fifo is empty
        while (spi_get_hw(spi0)->sr & 0x04) {
          uint8_t value = spi_get_hw(spi0)->dr;
          Spi_slave::Get()._read_fifo.push(value);
        }
      }

      // Tx fifo half empty or less
      if (status & SPI_SSPMIS_TXMIS_BITS) {
        // Adding 0xff if writefifo is empty
        spi_get_hw(spi0)->dr = Spi_slave::Get()._write_fifo.empty() ? 0xff : Spi_slave::Get()._write_fifo.front();
        if (!Spi_slave::Get()._write_fifo.empty()) Spi_slave::Get()._write_fifo.pop();
      }

      status = spi_get_hw(spi0)->mis;
    }
  }

}
