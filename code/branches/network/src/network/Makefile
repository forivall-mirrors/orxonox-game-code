CC=g++
MAIN_INC=..
INC=-I ${MAIN_INC}

all: server client

# Link command:
server: PacketBuffer.o ConnectionManager.o dummyserver.o PacketDecoder.o PacketGenerator.o
	${CC} PacketDecoder.o PacketGenerator.o ConnectionManager.o dummyserver.o PacketBuffer.o -o server -lenet -lboost_thread -g

dummyserver.o: dummyserver.cc
	${CC} -c dummyserver.cc -o dummyserver.o -g ${INC}
# Compilation commands:
ConnectionManager.o: ConnectionManager.cc
	${CC} -c ConnectionManager.cc -o ConnectionManager.o -g ${INC}

PacketBuffer.o: PacketBuffer.cc
	${CC} -c PacketBuffer.cc -o PacketBuffer.o -g ${INC}
PacketGenerator.o: PacketGenerator.cc
	${CC} -c PacketGenerator.cc -o PacketGenerator.o -g ${INC}
PacketDecoder.o: PacketDecoder.cc
	${CC} -c PacketDecoder.cc -o PacketDecoder.o -g ${INC}


clean:
	rm -rf *.o

client: PacketGenerator.o dummyclient.cc
	${CC} dummyclient.cc PacketGenerator.o -o client -lenet -g ${INC}
