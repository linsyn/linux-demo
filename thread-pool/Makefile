VPATH = src
CC = g++
SRCH_SRC_FILES = $(wildcard ./src/*.cc)
SRC_FILES = $(notdir $(SRCH_SRC_FILES))
OBJECTS = $(patsubst %.cc,%.o,$(SRC_FILES))
LDFLAGS = -pthread -g
TARGET = thread-pool.exx
all:$(OBJECTS)
	$(CC) $(LDFLAGS)  $(OBJECTS) -o $(TARGET)
$(OBJECTS):%.o:%.cc
	$(CC) -c $(CFLAGS) -g $< -o $@
.PHONY:clean
clean:
	rm -rf $(OBJECTS) $(TARGET)
