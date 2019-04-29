cmake_minimum_required(VERSION 3.7)
project(FT232H_SPI_NRF24 C)

set(CMAKE_C_STANDARD 99)

link_directories(lib)

add_executable(FT232H_SPI_NRF24 main.c nrf24.c nrf24.h nRF24L01.h nrf24_hal.c nrf24_hal.c)
target_link_libraries(FT232H_SPI_NRF24 MPSSE)
