USCORESUB = $(shell echo $1 | sed 'y; -./;____;')

export FLIRC_DIR := ./flircsdk

export MACHINE := $(call USCORESUB,$(shell uname -sm))

export FLIRC_CFLAGS = -I$(FLIRC_DIR)/libs/flirc -I$(FLIRC_DIR)/cli/include
export FLIRC_LIB = $(FLIRC_DIR)/libs/$(MACHINE)
export PYTHON3_CFLAGS := $(shell python3-config --cflags)

all: flircsdk pyflirc.so

clean:
	rm pyflirc.so pyflircmodule.o

flircsdk:
	git clone https://github.com/flirc/sdk.git flircsdk

pyflirc.so: pyflircmodule.o
	gcc -shared -fPIC -o pyflirc.so pyflircmodule.o $(FLIRC_LIB)/libflirc.a -lusb-1.0 -lhidapi-hidraw

pyflircmodule.o: pyflircmodule.c
	gcc -c $(FLIRC_CFLAGS) $(PYTHON3_CFLAGS) -fPIC -o pyflircmodule.o pyflircmodule.c

