LIBS=`pkg-config libpng --libs --cflags`
all:
	c++ -std=c++17 main.cpp $(LIBS) -o ptores1
install:
	install ptores1 /usr/local/bin
