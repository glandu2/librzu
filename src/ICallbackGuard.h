#ifndef CALLBACKGUARD_H
#define CALLBACKGUARD_H

#include <set>
#include <utility>
#include "RappelzLib_global.h"

typedef void** DelegateRef;

class RAPPELZLIB_EXTERN ICallbackGuard {
public:

	void addInstance(DelegateRef callbackValidityPtr) {
		if(callbackValidityPtr)
			callbackValidityPtrs.insert(callbackValidityPtr);
	}

	void delInstance(DelegateRef callbackValidityPtr) {
		callbackValidityPtrs.erase(callbackValidityPtr);
	}

	void invalidateCallbacks() {
		std::set<DelegateRef>::iterator it, itEnd;

		for(it = callbackValidityPtrs.begin(), itEnd = callbackValidityPtrs.end(); it != itEnd; ++it) {
			DelegateRef callbackValidityPtr = *it;
			*callbackValidityPtr = nullptr;
		}

		callbackValidityPtrs.clear();
	}

	~ICallbackGuard() {
		invalidateCallbacks();
	}


private:
	std::set<DelegateRef> callbackValidityPtrs;
};

#endif // CALLBACKGUARD_H
