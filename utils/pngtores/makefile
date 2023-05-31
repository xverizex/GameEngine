LIBS=`pkg-config libpng --libs --cflags`
all:
	c++ -std=c++17 main.cpp $(LIBS) -o ptores
install:
	install ptores /usr/local/bin
