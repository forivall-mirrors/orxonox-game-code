all: server client

# Link command:
server: clean PacketBuffer.o ConnectionManager.o dummyserver.o
	g++ ConnectionManager.o dummyserver.o PacketBuffer.o -o server -lenet -lboost_thread -g

dummyserver.o:
	g++ -c dummyserver.cc -o dummyserver.o -g
# Compilation commands:
ConnectionManager.o:
	g++ -c ConnectionManager.cc -o ConnectionManager.o -g

PacketBuffer.o: 
	g++ -c PacketBuffer.cc -o PacketBuffer.o -g

clean:
	rm -rf *.o

client:
	g++ dummyclient.cc -o client -lenet
