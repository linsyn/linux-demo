CC = gcc
VPATH = src
C_SRCH_SRC_FILES = $(wildcard ./src/c*.cc)
S_SRCH_SRC_FILES = $(wildcard ./src/s*.cc)
C_SRC_FILES = $(notdir $(C_SRCH_SRC_FILES))
C_OBJECTS = $(patsubst %.cc,%.o,$(C_SRC_FILES))
S_SRC_FILES = $(notdir $(S_SRCH_SRC_FILES))
S_OBJECTS = $(patsubst %.cc,%.o,$(S_SRC_FILES))
CLIENT_TARGET = client.exx
SERVER_TARGET = server.exx
all: client server
	
client: $(C_OBJECTS)
	$(CC) $(C_OBJECTS) -o $(CLIENT_TARGET)
server:  $(S_OBJECTS)
	$(CC) $(S_OBJECTS) -o $(SERVER_TARGET)
$(C_OBJECTS): %.o:%.cc
	$(CC) -c  $< -o $@
$(S_OBJECTS): %.o:%.cc
	$(CC) -c  $< -o $@
clean:
	rm -rf $(C_OBJECTS)  $(S_OBJECTS) $(SERVER_TARGET) $(CLIENT_TARGET)
