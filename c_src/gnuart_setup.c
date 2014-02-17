/*
 *
 * Copyright (C) 2014 
 * Authors: Patricia Nuñez cpattynue@gmail.com<>
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
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include "gnuart.h"
#include <string.h>
#include <syslog.h>


int open_uart_fd()
{
	// Initialize the UART file descriptor
	uart_fd = -1;

	// Open the uart file descriptor
    	uart_fd = open(devname, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
	
    	// Check if file descriptor has been correctly initialized
    	if (uart_fd == -1)
        {
		write_to_log("open NOK");
		return -1;
	} else {
		write_to_log("open OK");
		return uart_fd;
	}
}    


void close_uart_fd()
{
	write_to_log("close OK");
	close(uart_fd);
}


void configure_uart_fd() 
{

	if (uart_fd > -1) 
	{
		struct termios settings;

  		// Get configuration setup already set
  		tcgetattr(uart_fd, &settings);

  		fcntl(uart_fd, F_SETFL, 0);

  		// Set baud rates (default set to 9600)
  		cfsetispeed(&settings, B9600);
  		cfsetospeed(&settings, B9600);

  		settings.c_cflag |= (CLOCAL | CREAD);

  		// The next lines are to set other settings 
  		// settings.c_cflag &= ~PARENB;
  		// settings.c_cflag &= ~CSTOPB;
  		// settings.c_cflag &= ~CSIZE;
  		// settings.c_cflag |= CS8;

  		settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | IGNBRK);
  		settings.c_iflag &= ~(IGNPAR | IXON | IXOFF | IXANY);
  		settings.c_oflag &= ~OPOST;

		// Set timeout for non-blocking 
		settings.c_cc[VMIN] = vmin;
		settings.c_cc[VTIME] = vtimeout;

  		tcsetattr(uart_fd, TCSANOW, &settings);

		write_to_log("configure OK");
	} else {
		write_to_log("configure NOK");
	}
}


void load_config()
{
	FILE *fr;
   	char line[140];
	char cwd[256];	
	char config_file_path[256];

   	getcwd(cwd, sizeof(cwd));

   	strcpy(config_file_path, cwd);
   	strcat(config_file_path, CONFIG_FILE);

   	fr = fopen (config_file_path, MODE_FILE);

   	while(fgets(line, 140, fr) != NULL)
   	{
        	sscanf( line, "%s %d %d %d", devname, &vmin, &vmax, &vtimeout );
   	}

	write_to_log("load config file OK");
	write_to_log(devname);
	
   	fclose(fr); 
}


void write_to_log(char * function) {
	setlogmask(LOG_UPTO (LOG_NOTICE));
  	openlog(SYS_LOG, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
  	syslog(LOG_NOTICE, "[gnuart] %s UART file descriptor %d", function, uart_fd);
  	closelog();
}
