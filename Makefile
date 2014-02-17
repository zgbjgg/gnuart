##
## Copyright (C) 2014 
## Authors: Patricia Nuñez cpattynue@gmail.com<>
##          Jorge Garrido <zgbjgg@gmail.com>
## All rights reserved.
##
## This Source Code Form is subject to the terms of the Mozilla Public
## License, v. 2.0. If a copy of the MPL was not distributed with this
## file, You can obtain one at http://mozilla.org/MPL/2.0/.
##

##
##
## Necessary includes for build a port driver
##
ERL_ROOT=/usr/local/lib/erlang

##
## Shared object name 
##
SHARED_OBJ=gnuart.so

##
## Other CFLAGS
##
CFLAGSO=-shared -fpic -rdynamic

.PHONY: c_src

all: compile

c_src:
	$(CC) -o priv/$(SHARED_OBJ) $(CFLAGS) -I$(ERL_ROOT)/usr/include/ -I$(PWD)/c_src/ $(CFLAGSO) c_src/*.c

compile: c_src
	erlc -o ebin/ -I include/ src/*.erl

clean:
	@rm -rf priv/$(SHARED_OBJ)
	@rm -rf ebin/*.beam
