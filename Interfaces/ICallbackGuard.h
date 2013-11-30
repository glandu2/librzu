#ifndef CALLBACKGUARD_H
#define CALLBACKGUARD_H

#include <vector>
#include <utility>

class ICallbackGuard {
public:
	typedef void** CallbackPtr;

	void addInstance(CallbackPtr callbackValidityPtr) {
		if(callbackValidityPtr)
			callbackValidityPtrs.push_back(callbackValidityPtr);
	}

	void invalidateCallbacks() {
		std::vector<CallbackPtr>::iterator it, itEnd;

		for(it = callbackValidityPtrs.begin(), itEnd = callbackValidityPtrs.end(); it != itEnd; ++it) {
			CallbackPtr callbackValidityPtr = *it;
			*callbackValidityPtr = nullptr;
		}

		callbackValidityPtrs.clear();
	}

	~ICallbackGuard() {
		invalidateCallbacks();
	}


private:
	std::vector<CallbackPtr> callbackValidityPtrs;
};

#endif // CALLBACKGUARD_H
