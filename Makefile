CPP=g++ -std=c++17 -Wfatal-errors -Wextra -Wall -Wfloat-equal -Winit-self -Wuninitialized

.PHONY: all

all: prog.x


# program with curlpp
prog.x: main.cpp
	$(CPP) -O3 -o prog.x main.cpp -I/usr/local/include -L/usr/local/lib -lcurl -lcurlpp -lpthread

# Clean outputs.
clean:
	rm *.o *.x
