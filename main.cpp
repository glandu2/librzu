#include "Network/SocketPoll.h"
#include "Network/Socket.h"
#include <stdio.h>

SocketPoll socketPoll;

void onData(void*, ISocket* socket) {
	char buffer[1024];
	int nbread = socket->read(buffer, 1024);
	fwrite(buffer, 1, nbread, stdout);
	socket->write(buffer, nbread);
	socket->close();
	socketPoll.stop();
}

int main() {
	Socket s;

	printf("Ready\n");

	s.addDataListener(0, &onData);
	s.connect("127.0.0.1", 1233);
	socketPoll.run();
	printf("Done\n");
}
