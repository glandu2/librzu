#ifndef ILISTENER_H
#define ILISTENER_H

#include <vector>
#include "LibGlobal.h"

typedef void** DelegateRef;

class LIB_EXTERN IListener {
public:
	IListener() {}
	virtual ~IListener() {
		invalidateCallbacks();
	}

	void reserveCallbackCount(size_t count) {
		if(count)
			callbackValidityPtrs.reserve(callbackValidityPtrs.size() + count);
	}

	void addDelegateRef(DelegateRef callbackValidityPtr) {
		if(callbackValidityPtr)
			callbackValidityPtrs.push_back(callbackValidityPtr);
	}

	void delDelegateRef(DelegateRef callbackValidityPtr) {
		auto it = callbackValidityPtrs.begin();
		for(; it != callbackValidityPtrs.end(); ++it) {
			if(*it == callbackValidityPtr) {
				callbackValidityPtrs.erase(it);
				break;
			}
		}
	}

	void invalidateCallbacks() {
		auto it = callbackValidityPtrs.cbegin();
		auto itEnd = callbackValidityPtrs.cend();

		for(; it != itEnd; ++it) {
			DelegateRef callbackValidityPtr = *it;
			*callbackValidityPtr = nullptr;
		}

		callbackValidityPtrs.clear();
	}

private:
	std::vector<DelegateRef> callbackValidityPtrs;
};

#endif // ILISTENER_H
