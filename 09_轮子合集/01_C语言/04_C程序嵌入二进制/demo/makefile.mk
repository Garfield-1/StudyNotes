CC := gcc
SRCS := $(wildcard ./src/*.c)
TARGETS := $(patsubst %.c, %.o, $(SRCS))
CFLAGS := -g -Wall 
INC_DIR = -I ./inc/						#链接自定义头文件路径
LIB := -lm								#链接库 
#LDFLAGS = -L./lib -Wl,-rpath=./lib		#自定义动态库路径，并设置程序允许时从这个路径读取动态库
PRODUCT := a.out						#编译产物

all: $(PRODUCT)

$(PRODUCT): $(TARGETS)
	$(CC) $^ -o $@ $(LIB)
#	如果需要使用自定义的动态库，请使用下面的代码
#	$(CC) $^ -o $@ $(LDFLAGS)
	rm $(TARGETS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(INC_DIR) $< -o $@

.PHONY: clean
clean:
	rm $(TARGETS) $(PRODUCT)