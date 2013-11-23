#ifndef ISOCKET_H
#define ISOCKET_H

#include "IObject.h"
#include <string>
#include <stdint.h>

class ISocketListener;

class ISocket : public IObject
{
public:
	enum State {
		UnconnectedState,
		HostLookupState,
		ConnectingState,
		ConnectedState,
		ClosingState
	};

public:
	virtual bool IFACECALLCONV connect(const std::string & hostName, uint16_t port);
	virtual size_t IFACECALLCONV read(void *buffer, size_t size);
	virtual size_t IFACECALLCONV write(const void *buffer, size_t size);
	virtual void IFACECALLCONV close();
	virtual void IFACECALLCONV abort();
	virtual State IFACECALLCONV state();

	virtual void IFACECALLCONV addEventListener(ISocketListener *listener);
	virtual void IFACECALLCONV removeEventListener(ISocketListener *listener);
};

#endif // ISOCKET_H
