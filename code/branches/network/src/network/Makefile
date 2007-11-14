all: server client

# Link command:
server: clean PacketBuffer.o ConnectionManager.o dummyserver.o PacketDecoder.o PacketGenerator.o
	g++ PacketDecoder.o PacketGenerator.o ConnectionManager.o dummyserver.o PacketBuffer.o -o server -lenet -lboost_thread -g

dummyserver.o:
	g++ -c dummyserver.cc -o dummyserver.o -g
# Compilation commands:
ConnectionManager.o:
	g++ -c ConnectionManager.cc -o ConnectionManager.o -g

PacketBuffer.o: 
	g++ -c PacketBuffer.cc -o PacketBuffer.o -g
PacketGenerator.o:
	g++ -c PacketGenerator.cc -o PacketGenerator.o -g
PacketDecoder.o:
	g++ -c PacketDecoder.cc -o PacketDecoder.o -g


clean:
	rm -rf *.o

client: PacketGenerator.o
	g++ dummyclient.cc PacketGenerator.o -o client -lenet -g
