#include "TelnetSession.h"
#include "Core/Utils.h"
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>

EventChain<SocketSession> TelnetSession::onDataReceived() {
	std::vector<char> buffer;

	if(getStream()->getAvailableBytes() > 0) {
		getStream()->readAll(&buffer);
		parseData(buffer);
	}

	return SocketSession::onDataReceived();
}

void TelnetSession::parseData(const std::vector<char>& data) {
	buffer.insert(buffer.end(), data.begin(), data.end());

	char* nextLine;

	while((nextLine = (char*) memchr(buffer.data(), '\n', buffer.size()))) {
		size_t byteCount = nextLine - buffer.data() + 1;  //+1, count the found \n
		std::string line(buffer.data(), byteCount);
		parseCommand(line);
		buffer.erase(buffer.begin(), buffer.begin() + byteCount);
	}
}

void TelnetSession::parseCommand(const std::string& data) {
	std::vector<std::string> args = Utils::parseCommand(data);

	onCommand(args);
}
