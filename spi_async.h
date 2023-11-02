#ifndef __API_ASYNC_H__
#define __API_ASYNC_H__

enum spi_async_status {
	SPI_ASYNC_IDLE = 0,
	SPI_ASYNC_BUSY,
	SPI_ASYNC_BUFFER_OVERFLOW,
	SPI_ASYNC_COLLISION,
};

#endif // __API_ASYNC_H__