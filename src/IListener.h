#ifndef CALLBACKGUARD_H
#define CALLBACKGUARD_H

#include <unordered_set>
#include "RappelzLib_global.h"

typedef void** DelegateRef;

class RAPPELZLIB_EXTERN IListener {
public:

	void addDelegateRef(DelegateRef callbackValidityPtr) {
		if(callbackValidityPtr)
			callbackValidityPtrs.insert(callbackValidityPtr);
	}

	void delDelegateRef(DelegateRef callbackValidityPtr) {

		callbackValidityPtrs.erase(callbackValidityPtr);
	}

	void invalidateCallbacks() {
		std::unordered_set<DelegateRef>::iterator it, itEnd;

		for(it = callbackValidityPtrs.begin(), itEnd = callbackValidityPtrs.end(); it != itEnd; ++it) {
			DelegateRef callbackValidityPtr = *it;
			*callbackValidityPtr = nullptr;
		}

		callbackValidityPtrs.clear();
	}

	virtual ~IListener() {
		invalidateCallbacks();
	}


private:
	std::unordered_set<DelegateRef> callbackValidityPtrs;
};

#endif // CALLBACKGUARD_H
