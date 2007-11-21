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
ConnectionManager.o: ConnectionManager.cc ConnectionManager.h
	${CC} -c ConnectionManager.cc -o ConnectionManager.o -g ${INC}
ClientConnection.o: ClientConnection.cc ClientConnection.h
	${CC} -c ClientConnection.cc -o ClientConnection.o -g ${INC}

PacketBuffer.o: PacketBuffer.cc
	${CC} -c PacketBuffer.cc -o PacketBuffer.o -g ${INC}
PacketGenerator.o: PacketGenerator.cc
	${CC} -c PacketGenerator.cc -o PacketGenerator.o -g ${INC}
PacketDecoder.o: PacketDecoder.cc
	${CC} -c PacketDecoder.cc -o PacketDecoder.o -g ${INC}
Client.o: Client.cc
	${CC} -c Client.cc -g ${INC}
Server.o: Server.cc
	${CC} -c Server.cc -g ${INC}
Synchronisable.o: Synchronisable.cc
	${CC} -c Synchronisable.cc -g ${INC}

clean:
	rm -rf *.o

client_old: PacketGenerator.o dummyclient.cc
	${CC} dummyclient.cc PacketGenerator.o -o client -lenet -g ${INC}
client: PacketGenerator.o ClientConnection.o PacketBuffer.o dummyclient2.cc 
	${CC} dummyclient2.cc PacketGenerator.o PacketBuffer.o ClientConnection.o -o client -lenet -lboost_thread -g ${INC}
