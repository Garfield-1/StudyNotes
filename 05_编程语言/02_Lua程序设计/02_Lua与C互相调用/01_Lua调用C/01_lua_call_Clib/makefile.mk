CC := gcc
SRCS := $(wildcard ./src/*.c)
TARGETS := $(notdir $(patsubst %.c, %.so, $(SRCS)))
INC_DIR = -I ./inc/
CFLAGS := -Wall -fPIC -shared

$(TARGETS):
	$(CC) $(SRCS) $(INC_DIR) -o $(TARGETS) $(CFLAGS)

all :$(TARGETS)

.PHONY : clean
clean :
	rm $(TARGETS)