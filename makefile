
CC = g++
CFLAGS = -ljsoncpp -ltins -lpthread -std=c++11 -lboost_system -lcrypto -lssl -lcpprest -pthread


main: main.cpp capture_pkt.o save_pkt.o
	$(CC) -c main.cpp $(CFLAGS)
	$(CC) -o main_exe main.o capture_pkt.o save_pkt.o $(CFLAGS)

capture_pkt.o: capture_pkt.cpp
	$(CC) -c capture_pkt.cpp $(CFLAGS)


save_pkt.o: save_pkt.cpp
	$(CC) -c save_pkt.cpp $(CFLAGS)

clean:
	rm *.o *_exe

