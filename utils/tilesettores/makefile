LIBS=`pkg-config libpng --libs --cflags`
all:
	c++ -std=c++17 main.cpp $(LIBS) -o tileset_to_res
install:
	install tileset_to_res /usr/local/bin
