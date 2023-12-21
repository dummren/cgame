SRC = src/*.c
INCLUDE = -I./src/ -I./include/all/ -I./tmp/data/
FLAGS = -std=c17
DATA = $(shell find data/ -type f -name '*')
CGCONFIG = \
	# -DCG_STRANGE_TEXT \

OPTIMIZATION = -s -Oz -fdata-sections -ffunction-sections -fipa-pta \
	-Wl,--gc-sections -Wl,-s -Wl,-Oz -Wl,--as-needed -Wl,--strip-all

LIN64_CC = gcc
LIN64_INCLUDE = $(INCLUDE) -I./include/lin64/
LIN64_LIB = -lGL -lglfw -lopenal -lm
LIN64_SLIB =
LIN64_FLAGS = $(FLAGS) $(OPTIMIZATION)
LIN64_DEBUG_FLAGS = $(FLAGS) -g

WIN32_CC = i686-w64-mingw32-gcc
WIN32_INCLUDE = $(INCLUDE) -I./include/win32/
WIN32_LIB = -lopengl32 -lgdi32 -lglu32
WIN32_SLIB = lib/win32/*
WIN32_FLAGS = $(FLAGS) -g -static $(OPTIMIZATION) -Wl,-rpath=.

all: clean struct data lin64-debug
release: clean struct data lin64 win32 optimize-size

clean:
	rm -rf bin/ tmp/

struct:
	mkdir -p bin/ tmp/data/

.PHONY: data
data:
	python3 util/pack_assets.py
	xxd -i tmp/data/data.dat > tmp/data/data.dat.h

lin64:
	$(LIN64_CC) -o bin/cgame $(CGCONFIG) $(SRC) $(LIN64_SLIB) \
		$(LIN64_INCLUDE) $(LIN64_LIB) $(LIN64_FLAGS)

lin64-debug:
	$(LIN64_CC) -o bin/cgame $(CGCONFIG) $(SRC) $(LIN64_SLIB) \
		$(LIN64_INCLUDE) $(LIN64_LIB) $(LIN64_DEBUG_FLAGS)

win32:
	$(WIN32_CC) -o bin/cgame.exe $(CGCONFIG) $(SRC) $(WIN32_SLIB) \
		$(WIN32_INCLUDE) $(WIN32_LIB) $(WIN32_FLAGS)

	cp dlib/win32/* bin/

optimize-size:
	upx bin/* --all-methods --best --ultra-brute --lzma
