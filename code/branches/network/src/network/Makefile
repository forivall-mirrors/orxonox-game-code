CC=g++
MAIN_INC=..
INC=-I ${MAIN_INC}

all: server client

# Link command:
server: clean PacketBuffer.o ConnectionManager.o dummyserver.o PacketDecoder.o PacketGenerator.o
	g++ PacketDecoder.o PacketGenerator.o ConnectionManager.o dummyserver.o PacketBuffer.o -o server -lenet -lboost_thread -g

dummyserver.o:
	g++ -c dummyserver.cc -o dummyserver.o -g ${INC}
# Compilation commands:
ConnectionManager.o:
	g++ -c ConnectionManager.cc -o ConnectionManager.o -g ${INC}

PacketBuffer.o: 
	g++ -c PacketBuffer.cc -o PacketBuffer.o -g ${INC}
PacketGenerator.o:
	g++ -c PacketGenerator.cc -o PacketGenerator.o -g ${INC}
PacketDecoder.o:
	g++ -c PacketDecoder.cc -o PacketDecoder.o -g ${INC}


clean:
	rm -rf *.o

client: PacketGenerator.o
	g++ dummyclient.cc PacketGenerator.o -o client -lenet -g ${INC}
