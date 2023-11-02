# UART<->SPI interface library

This is a library for interfacing between UART (Universal Asynchronous Receiver/Transmitter) and SPI (Serial Peripheral Interface) on AVR microcontrollers. It allows for asynchronous communication between UART and SPI peripherals.

## UART Features

- Initialize UART with a specified baud rate.
- Asynchronous transmission and reception.
- Detection and handling of UART errors (frame error, data overrun, parity error).
- UART interrupt-driven transmission and reception.
- Non-blocking UART data transmission with a transmit queue.
- Non-blocking UART data reception with a receive queue.

## SPI Features

- Initialize SPI with a specified mode and baud rate.
- Asynchronous SPI communication.
- Detection and handling of SPI error (data collision)
- SPI interrupt-driven transmission and reception.
- Non-blocking SPI data transmission with a transmit queue.
- Non-blocking SPI data reception with a receive queue.
