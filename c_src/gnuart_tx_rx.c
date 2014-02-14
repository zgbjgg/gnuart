/*
 *
 * Copyright (C) 2014 
 * Authors: Patricia Nuñez cpattynue@gmail.com>
 *          Jorge Garrido <zgbjgg@gmail.com>
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <gnuart.h>
#include <syslog.h>

int write_uart_fd(unsigned char * tx_buffer, size_t size_tx_buffer) 
{

	// Check if file descriptor is available
	if (uart_fd > -1) 
	{
		// Write on file descriptor
		int retcount = write(uart_fd, tx_buffer, size_tx_buffer);
		write_to_log("write OK");
		return retcount;
	} else {
		
		// Means that non writable file descriptor
		write_to_log("write NOK");
		return -1;
	}
}

unsigned char * read_uart_fd(int timeout)
{
	if (uart_fd > -1)
	{
		int waitime = 0;	

		// Loop for checking the remains bytes on buffer, maybe a 
		// escape buffer of 10 bytes on each sequence for that reason skip the first
		// ten bytes
		// Receive bytes < 0  - error reading
		// Receive bytes == 0 - read all bytes ready to retrieve data
		// Receive bytes > 0  - there are remain bytes to read
		while (waitime < timeout)
		{
			// Read on file descriptor
			if (uart_fd != -1)
			{
				unsigned char rx_buffer[256];
                        	int rx_length = read(uart_fd, rx_buffer, 255);

				if (rx_length < 0) 
				{
					return rx_buffer;
				} else if (rx_length == 0)
				{
					return rx_buffer;
				} else {
					rx_buffer[rx_length] = '\0';
				}
				sleep(1);
				waitime += 1;
			} else {
				return "6F00";
			} 
		}
		write_to_log("read OK");
	} else {
		
		// Means that non redable file descriptor
		write_to_log("read NOK");
		return "6F00";
	}
}

