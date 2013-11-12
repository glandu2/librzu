#ifndef SOCKET_H
#define SOCKET_H

#include "ISocket.h"
#include <list>
#include <pthread.h>

class Socket : public CImplement<ISocket>
{
public:
	Socket();
	virtual ~Socket();

	bool IFACECALLCONV connect(const std::string & hostName, uint16_t port);
	size_t IFACECALLCONV read(void *buffer, size_t size);
	size_t IFACECALLCONV write(const void *buffer, size_t size);
	void IFACECALLCONV close();
	void IFACECALLCONV abort();
	State IFACECALLCONV state();

	void IFACECALLCONV addEventListener(ISocketListener *listener);
	void IFACECALLCONV removeEventListener(ISocketListener *listener);

protected:
	static void *pollThread(void*);
	static pthread_t pollthreadId;
	static std::list<Socket *> sockets;
	static pthread_mutex_t mutex;

private:
	std::list<ISocketListener *> listeners;
	int sock;
	State currentState;
};

#endif // SOCKET_H
