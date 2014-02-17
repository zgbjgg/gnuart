gnuart - the UART erlang application
======

Compile the application and build the shared object

	$ make all

Before start application, you must configure priv/gnuart.uart file. This file is the configuration where 
the devname must be set. The order of the specs is:

	devname minimun_time maximum_time timeout

The specs can be changed at the runtime and could be reloaded when write data to devname.

Start the application with the code loaded 

	$ erl -pa ebin/
	
	1> application:start(gnuart).
	ok
	
From a valid process, subscribe it to gnuart

	2> gnuart:subscribe().
	{ok,#Ref<0.0.0.39>}
	
Send bytes

	3> gnuart:flush("0s").
	{ok,flush}
	
The response is delivered to the calling process (subscribed)

	4> flush().
	Shell got {got,#Ref<0.0.0.39>,"BAD_FILE_DESCRIPTOR"}
	ok
	
However you can unsubscribe a process

	5> gnuart:unsubscribe(Ref).
	{ok,unsubscribe}
	
	
License
===
See LICENSE.txt for more information
