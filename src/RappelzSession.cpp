#include "RappelzSession.h"
#include "EventLoop.h"
#include "RappelzServer.h"
#include "stdarg.h"

RappelzSession::RappelzSession(EncryptedSocket::Mode mode) : SocketSession(new RappelzSocket(EventLoop::getLoop(), mode)) {
}

void RappelzSession::addPacketsToListen(int packetsIdNum, int firstPacketId, ...) {
	va_list l;
	va_start(l, firstPacketId);
	reserveCallbackCount(packetsIdNum);
	for(int i = 0; i < packetsIdNum; i++) {
		if(i != 0)
			getSocket()->addPacketListener(va_arg(l, int), this, &onDataReceived);
		else
			getSocket()->addPacketListener(firstPacketId, this, &onDataReceived);
	}
	va_end(l);
}

void RappelzSession::onDataReceived(IListener* instance, RappelzSocket*, const TS_MESSAGE* packet) {
	static_cast<RappelzSession*>(instance)->onPacketReceived(packet);
}

void RappelzSession::onPacketReceived(const TS_MESSAGE*) {
}
