LIBS=`pkg-config libpng --libs --cflags`
all:
	c++ -std=c++17 main.cpp $(LIBS) -I/usr/local/include -o pbr_to_res
install:
	install pbr_to_res /usr/local/bin
