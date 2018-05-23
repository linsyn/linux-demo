VPATH = src
SRCH_SRC_FILES = $(wildcard ./src/*.cc)
SRC_FILES = $(notdir $(SRCH_SRC_FILES))
OBJECT = $(patsubst %.cc,%.o,$(SRC_FILES))
TARGET = process_pool.exx
CFLAGS = -g
CC = g++
all: $(OBJECT)
	$(CC) $(OBJECT) -o $(TARGET)
$(OBJECT): %.o:%.cc
	$(CC) -c $(CFLAGS) $< -o $@
.PHONY:clean
clean:
	rm -rf $(TARGET)  $(OBJECT)
