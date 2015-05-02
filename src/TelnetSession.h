#ifndef TELNETSESSION_H
#define TELNETSESSION_H

#include "SocketSession.h"
#include <string>
#include <sstream>

class RZU_EXTERN TelnetSession : public SocketSession
{
	DECLARE_CLASS(TelnetSession)
public:
	TelnetSession() {}
	virtual ~TelnetSession() {}

	virtual void onConnected() {}
	virtual void onCommand(const std::vector<std::string>& args) = 0;

protected:
	virtual void write(const void* data, int size) { if(getStream()->getState() == Stream::ConnectedState) getStream()->write(data, size); }
	virtual void close() { getStream()->abort(); }

private:
	void onDataReceived();

	void parseData(const std::vector<char>& data);
	void parseCommand(const std::string& data);

private:
	std::vector<char> buffer;
};

#endif // TELNETSESSION_H
