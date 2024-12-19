CC := gcc
SRCS := $(wildcard ./src/*.c)
TARGETS := $(patsubst %.c, %.o, $(SRCS))
CFLAGS := -g -Wall 
INC_DIR = -I ./inc/                     #链接自定义头文件路径
LIB := -lm                              #链接库 
LDFLAGS = -L./lib -Wl,-rpath=./lib      #自定义动态库路径，并设置程序允许时从这个路径读取动态库
EXEC_BIN := a.out                       #编译产物,可执行文件
EXEC_LIB := liba.so                     #编译产物,动态库

all: $(EXEC_BIN) $(EXEC_LIB)
	rm $(TARGETS)

$(EXEC_BIN): $(TARGETS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LIB)
	
$(EXEC_LIB): $(TARGETS)
	$(CC) -shared -fPIC -o $@ $(TARGETS)

$(TARGETS): %.o: %.c
	$(CC) -c $(CFLAGS) $(INC_DIR) $< -o $@

.PHONY: clean
clean:
	rm $(EXEC_BIN) $(EXEC_LIB)