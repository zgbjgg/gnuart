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


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "gnuart.h"
#include <stdlib.h>
#include "erl_driver.h"
#include <syslog.h>


static ErlDrvData gnuart_driver_start(ErlDrvPort port, char *buff)
{
	stct_port* to_erl_port = (stct_port*)driver_alloc(sizeof(port));
    	to_erl_port->port = port;
    	return (ErlDrvData)to_erl_port;
}

static void gnuart_driver_stop(ErlDrvData handle)
{
    	driver_free((char*)handle);
}

static void gnuart_driver_output(ErlDrvData handle, char *buff, ErlDrvSizeT bufflen)
{
    	stct_port* to_erl_port = (stct_port*)handle;
    	unsigned char * tx_buffer = buff;

    	// Open devname & configure
    	open_uart_fd("/dev/ttySAC3");
        configure_uart_fd();
         
        // Write
       	int wuart_fd = write_uart_fd(tx_buffer, strlen(tx_buffer));
	
	if (uart_fd == -1)
  	{
		unsigned char * rx_buffer = "BAD_FILE_DESCRIPTOR";
        	driver_output(to_erl_port->port, rx_buffer, strlen(rx_buffer));
	} else {
       
    		// Read 
    		unsigned char * rx_buffer = read_uart_fd(5);
    		driver_output(to_erl_port->port, rx_buffer, strlen(rx_buffer));
	}
}

ErlDrvEntry gnuart_driver_entry = {
        NULL,                           /* F_PTR init, N/A */
        gnuart_driver_start,            /* L_PTR start, called when port is opened */
        gnuart_driver_stop,             /* F_PTR stop, called when port is closed */
        gnuart_driver_output,           /* F_PTR output, called when erlang has sent */
        NULL,                           /* F_PTR ready_input, called when input descriptor ready */
        NULL,                           /* F_PTR ready_output, called when output descriptor ready */
        "gnuart",   	                /* char *driver_name, the argument to open_port */
        NULL,                           /* F_PTR finish, called when unloaded */
        NULL,
        NULL,                           /* F_PTR control, port_command callback */
        NULL,                           /* F_PTR timeout, reserved */
        NULL,                           /* F_PTR outputv, reserved*/
        NULL,                           /* F_PTR ready_async, only for async drivers */
        NULL,                           /* F_PTR flush, called when port is about
                                           to be closed, but there is data in driver
                                           queue */
        NULL,                           /* F_PTR call, much like control, sync call
                                           to driver */
        NULL,                           /* F_PTR event, called when an event selected
                                           by driver_event() occurs. */
        ERL_DRV_EXTENDED_MARKER,        /* int extended marker, Should always be
                                           set to indicate driver versioning */
        ERL_DRV_EXTENDED_MAJOR_VERSION, /* int major_version, should always be
                                           set to this value */
        ERL_DRV_EXTENDED_MINOR_VERSION, /* int minor_version, should always be
                                           set to this value */
        0,                              /* int driver_flags, see documentation */
        NULL,                           /* void *handle2, reserved for VM use */
        NULL,                           /* F_PTR process_exit, called when a
                                           monitored process dies */
        NULL                            /* F_PTR stop_select, called to close an
                                           event object */
};


DRIVER_INIT(gnuart) /* must match name in driver_entry */
{
    return &gnuart_driver_entry;
}
