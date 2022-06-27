SPACE = echo -e "\n\n\n\n\n";

CC = $(SPACE) gcc 

CFLAGS = -std=c99 -pedantic-errors -Wall -Wextra -DNDEBUG -O3 # gc flags

DEBUG = -std=c99  -Wall -Wextra -g -pedantic-errors

ifeq ($(DEB), debug)
CFLAGS = $(DEBUG)
endif

TARGET = calc

HEARDER = calc.h

SHARED_OBJ = lib$(TARGET).so

SERVER_PATH = /home/meiravgri/Redis/redis-7.0.0/src/redis-server

RUN_SERVER_CMD = $(SERVER_PATH) --loadmodule $(PWD)/$(SHARED_OBJ) 

$(SHARED_OBJ): $(TARGET).o $(HEADER) 
	$(CC) $(CFLAGS) -shared $(TARGET).o -o $(SHARED_OBJ)


$(TARGET).o : $(TARGET).c $(HEADER) 
	$(CC) $(CFLAGS) -fpic -c $(TARGET).c 

run_server:
	$(RUN_SERVER_CMD) 

test_module:
	make
	$(RUN_SERVER_CMD)

clean:
	rm $(TARGET).o $(SHARED_OBJ)

