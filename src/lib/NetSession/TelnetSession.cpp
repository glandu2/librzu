#include "TelnetSession.h"
#include <string.h>
#include <stdio.h>
#include "Core/Utils.h"
#include <string>
#include <sstream>

void TelnetSession::onDataReceived() {
	std::vector<char> buffer;

	if(getStream()->getAvailableBytes() > 0) {
		getStream()->readAll(&buffer);
		parseData(buffer);
	}
}

void TelnetSession::parseData(const std::vector<char>& data) {
	buffer.insert(buffer.end(), data.begin(), data.end());

	char* nextLine;

	while((nextLine = (char*)memchr(buffer.data(), '\n', buffer.size()))) {
		size_t byteCount = nextLine - buffer.data() + 1; //+1, count the found \n
		std::string line(buffer.data(), byteCount);
		parseCommand(line);
		buffer.erase(buffer.begin(), buffer.begin() + byteCount);
	}
}

void TelnetSession::parseCommand(const std::string& data) {
	std::vector<std::string> args;
	std::ostringstream arg;

	const char *p;
	bool insideQuotes = false;

	for(p = data.c_str(); p < data.c_str() + data.size(); p++) {
		if(*p == '\"') {
			if(p+1 < data.c_str() + data.size() && *(p+1) == '\"') {
				p++;
				arg << '\"';
			} else {
				insideQuotes = !insideQuotes;
			}
		} else if(insideQuotes == false && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')) {
			if(arg.tellp()) {
				args.push_back(arg.str());
				arg.str("");
				arg.clear();
			}
		} else {
			arg << *p;
		}
	}
	if(arg.tellp())
		args.push_back(arg.str());

	onCommand(args);
}
