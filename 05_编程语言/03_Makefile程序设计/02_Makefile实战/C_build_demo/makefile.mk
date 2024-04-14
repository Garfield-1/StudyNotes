CC := gcc
SRCS := $(wildcard ./src/*.c)
TARGETS := $(patsubst %.c, %.o, $(SRCS))
CFLAGS := -g -Wall 
INC_DIR = -I ./inc/				#链接自定义头文件路径
LIB := -lm						#链接库 
PRODUCT := a.out				#编译产物

all : $(PRODUCT)

$(PRODUCT) : $(TARGETS)
	$(CC) $(TARGETS) -o $(PRODUCT) $(LIB)
	rm $(TARGETS)

$(TARGETS) : $(SRCS)
	$(CC) -c $(CFLAGS) $(patsubst %.o, %.c, $@) -o  $@ $(INC_DIR)

.PHONY : clean
clean :
	rm $(TARGETS) $(PRODUCT)