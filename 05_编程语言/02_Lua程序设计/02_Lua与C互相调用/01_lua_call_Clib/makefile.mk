CC := gcc
SRCS := $(wildcard ./src/*.c)
OBJS := $(SRCS:.c=.o)
INC_DIR = -I ./inc/
CFLAGS := -Wall -fPIC

# 使用 pkg-config 自动探测 LUA_PKG（仍支持外部覆盖）
PKG_CONFIG ?= pkg-config
LUA_CANDIDATES := lua5.4 lua5.3 lua5.2 luajit lua
LUA_PKG ?= $(firstword $(foreach m,$(LUA_CANDIDATES),$(if $(shell $(PKG_CONFIG) --exists $(m) && echo yes),$(m))))
ifeq ($(strip $(LUA_PKG)),)
$(error 未找到可用的 Lua 开发包，请安装 liblua5.x-dev 或手动设置 LUA_PKG=lua5.4 等)
endif
$(info Using LUA_PKG=$(LUA_PKG))

PKG_CFLAGS := $(shell $(PKG_CONFIG) --cflags $(LUA_PKG))
PKG_LIBS   := $(shell $(PKG_CONFIG) --libs   $(LUA_PKG))

PRODUCT := mylib.so

all: $(PRODUCT)

$(PRODUCT): $(OBJS)
	$(CC) -shared -o $@ $^ $(PKG_LIBS)

src/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INC_DIR) $(PKG_CFLAGS)

.PHONY: clean
clean:
	rm -f $(PRODUCT) $(OBJS)