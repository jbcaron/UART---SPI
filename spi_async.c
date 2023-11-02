/*
 * spi_async.c
 *
 * description: spi asynchroneous driver for the atmega328p microcontroller.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "queue/queue.h"
#include "spi_async.h"

/* macros =================================================================== */

#define spi_ready_to_send() (SPSR & (1 << SPIF))
#define spi_trigger_interrupt() (SPCR |= (1 << SPIE))

/* private variables ======================================================== */

volatile Queue spi_send_queue = queue_init();
volatile Queue spi_receive_queue = queue_init();

/* public variables ========================================================= */

volatile enum spi_async_status spi_status = SPI_ASYNC_IDLE;

/* publics functions ======================================================== */

void spi_async_init(uint8_t mode, uint16_t baudrate) {
	// set MOSI, SCK and SS as output
	SPI_DDR |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
	
	// set MISO as input
	SPI_DDR &= ~(1 << SPI_MISO);
	
	// enable SPI, set as master
	SPCR |= (1 << SPE) | (1 << MSTR);
	
	// set SPI mode
	SPCR &= ~(1 << CPOL) & ~(1 << CPHA);
	SPCR |= (mode & 0x03);

	// set baudrate
	uint8_t baudrate_register = (uint8_t) (F_CPU / (2 * baudrate) - 1);
	SPCR &= ~(1 << SPR1);
	SPCR &= ~(1 << SPR0);
	SPCR |= (baudrate_register & 0x03);
	SPSR &= ~(1 << SPI2X);
	SPSR |= ((baudrate_register >> 2) & 0x01);
	
	// enable SPI interrupt
	SPCR |= (1 << SPIE);
	
	// enable global interrupts
	sei();
}

// SPI interrupt service routine when a byte has been sent
ISR (SPI_STC_vect) {
	uint8_t data;

	// if a collision has occured, do nothing
	if (SPSR & (1 << WCOL))
		spi_status = SPI_ASYNC_COLLISION;

	// if there is data to receive, receive it
	else if (spi_status = SPI_ASYNC_BUSY && queue_push(&spi_receive_queue, SPDR) == QUEUE_FULL)
		spi_status = SPI_ASYNC_BUFFER_OVERFLOW;

	// if there is data to send, send it
	else if (queue_pop(&spi_send_queue, &data) == QUEUE_OK)
	{
		SPDR = data;
		spi_status = SPI_ASYNC_BUSY;
	}

	// if there is no data to send, set the status to idle
	else
		spi_status = SPI_ASYNC_IDLE;
}

void spi_async_transmit(uint8_t data) {
	if (queue_push(&spi_send_queue, data) == QUEUE_FULL)
		spi_status = SPI_ASYNC_BUFFER_OVERFLOW;
	else if (spi_async_status == SPI_ASYNC_IDLE)
		spi_trigger_interrupt();
}
