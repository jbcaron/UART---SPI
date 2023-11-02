#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "queue/queue.h"

/* macros =================================================================== */

#define uart_trigger_interrupt() (UCSR0B |= (1 << UDRIE0))

/* private variables ======================================================== */

volatile Queue uart_send_queue = queue_init();
volatile Queue uart_receive_queue = queue_init();

/* public variables ========================================================= */

volatile uart_async_status uart_status = UART_ASYNC_IDLE;

/* publics functions ======================================================== */

void uart_init(uint16_t baudrate) {
	// set TX as output
	UART_DDR |= (1 << UART_TX);
	
	// set RX as input
	UART_DDR &= ~(1 << UART_RX);
	
	// set baudrate
	uint16_t baudrate_register = (uint16_t) (F_CPU / (16 * baudrate) - 1);
	UBRR0H = (uint8_t) (baudrate_register >> 8);
	UBRR0L = (uint8_t) baudrate_register;
	// set double speed mode if necessary
	if (baudrate_register & 0x01)
		UCSR0A |= (1 << U2X0);
	else
		UCSR0A &= ~(1 << U2X0);
	

	// enable receiver and transmitter
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	
	// set frame format: 8 data, 1 stop bit
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

	// enable RX & TX interrupts
	UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0);

	// enable global interrupts
	sei();

	// set status to idle
	uart_status = UART_ASYNC_IDLE;
}

// UART interrupt service routine when a byte has been sent
ISR (USART_TX_vect) {
	uint8_t data;

	// error detection
	if (UCSR0A & ((1 << FE0))) 
		uart_async_status = UART_ASYNC_FRAME_ERROR;
	else if (UCSR0A & ((1 << UPE0)))
		uart_async_status = UART_ASYNC_PARITY_ERROR;

	else if (queue_pop(&uart_send_queue, &data) == QUEUE_OK)
	{
		UDR0 = data;
		uart_status = UART_ASYNC_TX_BUSY;
	}
	else
		uart_status = UART_ASYNC_TX_IDLE;
}

// UART interrupt service routine when a byte has been received
ISR (USART_RX_vect) {
	uint8_t data;

	// error detection
	if (UCSR0A & ((1 << FE0))) 
		uart_async_status = UART_ASYNC_FRAME_ERROR;
	else if (UCSR0A & ((1 << DOR0)))
		uart_async_status = UART_ASYNC_DATA_OVERRUN;
	else if (UCSR0A & ((1 << UPE0)))
		uart_async_status = UART_ASYNC_PARITY_ERROR;

	else if (queue_push(&uart_receive_queue, UDR0) == QUEUE_FULL)
		uart_async_status = UART_ASYNC_OVERFLOW;
}

void uart_async_transmit(uint8_t data) {
	if (queue_push(&uart_send_queue, data) == QUEUE_FULL)
		uart_status = UART_ASYNC_OVERFLOW;
	else if (uart_status == UART_ASYNC_TX_IDLE)
		uart_trigger_interrupt();
}