    C ?= gcc

CFLAGS = -g -O2 -Wall -I$(LUA_INC)

    SHARED := -fPIC --shared

    TARGET = Luaclib.so

    LUA_CLIB_PATH = clib

# 引入lua头文件

    LUA_INC ?= ./include

    start: $(TARGET)

    $(TARGET) : Luaclib.c | $(LUA_CLIB_PATH)

    $(CC) $(CFLAGS) $(SHARED) $^ -o $@

    clean:

    rm -fr $(TARGET)

    $(LUA_CLIB_PATH) :

    mkdir $(LUA_CLIB_PATH)
