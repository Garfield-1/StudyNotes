CC := gcc
SRCS := $(wildcard ./src/*.c)
TARGETS := $(patsubst %.c, %.o, $(SRCS))
INC_DIR = -I ./inc/
CFLAGS := -Wall -fPIC -shared
PRODUCT := mylib.so						#编译产物

all : $(PRODUCT)

$(PRODUCT) : $(TARGETS)
	$(CC) -shared -o $(PRODUCT) $(TARGETS)
	rm $(TARGETS)

$(TARGETS) : $(SRCS)
	$(CC) -c $(patsubst %.o, %.c, $@) -o  $@ $(INC_DIR)

.PHONY : clean
clean :
	rm $(PRODUCT) $(TARGETS)