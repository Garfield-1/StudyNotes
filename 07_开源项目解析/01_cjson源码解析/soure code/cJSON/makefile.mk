SRCS := $(wildcard *.c)
TARGETS := $(patsubst %.c, %.o, $(SRCS))
CFLAGS := -g -Wall 
LIB := -lm
PRODUCT := a.out

all : build

build : $(TARGETS)
	$(CC) $(TARGETS) -o $(PRODUCT) $(LIB)
	rm $(TARGETS)

$(TARGETS) : $(SRCS)
	gcc -c $(CFLAGS) $(patsubst %.o, %.c, $@) -o  $@

.PHONY : clean
clean :
	rm $(TARGETS) $(PRODUCT)