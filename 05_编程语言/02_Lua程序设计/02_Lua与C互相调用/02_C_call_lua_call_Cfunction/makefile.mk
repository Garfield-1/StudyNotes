CC := gcc
SRCS := $(wildcard *.c)
CFLAGS := -g -Wall
PRODUCT := a.out					#编译产物
LIB := -llua -lm -ldl

all : $(PRODUCT)

$(PRODUCT) : $(SRCS)
	$(CC) $^ $(CFLAGS) -o $(PRODUCT) $(LIB)

.PHONY : clean
clean :
	rm $(PRODUCT) $(TARGETS)