#pragma once

#include "../Extern.h"
#include <type_traits>

template<class RootEventHandlerClass> class EventChain {
public:
	EventChain(EventChain&&) {}

private:
	friend RootEventHandlerClass;

	EventChain() {}
	EventChain(const EventChain&);
	EventChain& operator=(const EventChain&);
};

