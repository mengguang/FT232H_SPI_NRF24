//
// Created by mengguang on 2019/4/28.
//

/* Standard C libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
/* OS specific libraries */
#ifdef _WIN32

#include <windows.h>

#endif

/* Include D2XX header*/
#include "ftd2xx.h"

/* Include libMPSSE header */
#include "libMPSSE_spi.h"


#define APP_CHECK_STATUS(exp) {if(exp!=FT_OK){printf("%s:%d:%s(): status(0x%lx) \
!= FT_OK\n",__FILE__, __LINE__, __FUNCTION__,exp);exit(1);}else{;}};

FT_STATUS status = FT_OK;

static uint8_t gpio_output_status = 0x00;

extern FT_HANDLE ftHandle;

#define HIGH    1
#define LOW     0

#define NRF24_CS_PIN    0
#define NRF24_CE_PIN    1

void gpio_init() {
    gpio_output_status = 0x00;
    status = FT_WriteGPIO(ftHandle, 0xFF, gpio_output_status);
    APP_CHECK_STATUS(status);
}

void digitalWrite(uint8_t pin, uint8_t value) {
    if (value == HIGH) {
        gpio_output_status |= (1 << pin);
    } else {
        gpio_output_status &= ~(1 << pin);
    }
    status = FT_WriteGPIO(ftHandle, 0xFF, gpio_output_status);
    APP_CHECK_STATUS(status);
}

uint8_t spi_transfer(uint8_t tx) {
    uint8_t rx;
    uint32_t n;
    status = SPI_ReadWrite(ftHandle, &rx, &tx, 1, &n,0);
    return rx;
}

/* -------------------------------------------------------------------------- */
/* In this function you should do the following things:
 *    - Set MISO pin input
 *    - Set MOSI pin output
 *    - Set SCK pin output
 *    - Set CSN pin output
 *    - Set CE pin output     */
/* -------------------------------------------------------------------------- */
void nrf24_setupPins() {
    gpio_init();
}

/* -------------------------------------------------------------------------- */
/* nrf24 CE pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(uint8_t state) {
    digitalWrite(NRF24_CE_PIN, state);
}

/* -------------------------------------------------------------------------- */
/* nrf24 CE pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
void nrf24_csn_digitalWrite(uint8_t state) {
    digitalWrite(NRF24_CS_PIN, state);
}
