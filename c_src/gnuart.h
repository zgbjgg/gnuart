/*
 *
 * Copyright (C) 2014
 * Authors: Patricia Nuñez <cpattynue@gmail.com>
 *          Jorge Garrido <zgbjgg@gmail.com>
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

/*
** Include this file for erlang struct definitions, used for define 
** the struct ErlDrvPort. To use it, add the include path to compiler.
*/
#include "erl_driver.h"

/*
** Struct for erlang port, send/receive data from/to erlang
*/
typedef struct {
    ErlDrvPort port;
} stct_port;

/*
** UART file descriptor, represented as signed integer.
*/
int uart_fd;

/*
** The minimun time to wait response from device.
*/
int vmin;

/*
** The maximum time to wait response from device.
*/
int vmax;

/* 
** Timeout device
*/
int vtimeout;

/*
** The device where UART must be open.
*/
char devname[12];

/*
** Define the location of the configuration file, 'cause this 
** application can be used as a simple app or include on another
** for that reason config file is placed under priv directory of the 
** main root, from this applications is started.
*/
#define CONFIG_FILE "/priv/gnuart.uart"

/*
** The file's mode to be opened. Just read.
*/
#define MODE_FILE "rt"

/*
** Open device tag and close device tag
*/
#define OPEN_DEVICE 7

#define CLOSE_DEVICE 8

/*
** Location of system log messages, default set to 
** '/var/log/messages'
*/
#define SYS_LOG "/var/log/messages"

/*
** \brief Open a file descriptor on devname specified on configuration
**        file. The opened fd is suitable for write and read data.
** 
** \param none
** 
** \return the uart file descriptor for the device status
*/
unsigned char * open_uart_fd();

/*
** \brief Write messages to system logs, for more info.
**
** \param [char *] a string message
**
** \return void 
*/
void write_to_log(char * function);

/*
** \brief Configure the uart file descriptor, the configurations
**        are a set of specs for more control. See the logs for 
**	  information about configuration state.
** 
+* \param none
**
** \return void
*/
void configure_uart_fd();

/*
** \brief Close the uart file descriptor
**
** \param Close the file descriptor on devname specified on configuration
**	  file. The fd must be a suitable file descriptor.
**
** \return the status for the closed fd
*/
unsigned char * close_uart_fd();


/*
** \brief Load the configuration file.
**
** \param none
**
** \return void
*/
void load_config();

/*
** \brief Reads the buffer of data returned for the device on the 
**	  opened file descriptor. Read data until buffer length is zero
**	  or negative number. Returns a char containing the remains buffer.
**
** \param [int] the timeout to wait a valid response
**
** \return [unsigned char *]  A string representing the buffer or read bytes.
*/
unsigned char * read_uart_fd(int timeout);

/*
** \brief Writes data to devname using the opened file descriptor.
**
** \param [unsigned char *] the string to write
** \param [size_t]	    the length of the string to write
**
** \return [int] the response for the write operation on the file descriptor.
*/
int write_uart_fd(unsigned char * tx_buffer, size_t size_tx_buffer);
