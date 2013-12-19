#ifndef CALLBACKGUARD_H
#define CALLBACKGUARD_H

#include <vector>
#include <utility>

typedef void** DelegateRef;

class ICallbackGuard {
public:

	void addInstance(DelegateRef callbackValidityPtr) {
		if(callbackValidityPtr)
			callbackValidityPtrs.push_back(callbackValidityPtr);
	}

	void invalidateCallbacks() {
		std::vector<DelegateRef>::iterator it, itEnd;

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
	std::vector<DelegateRef> callbackValidityPtrs;
};

#endif // CALLBACKGUARD_H