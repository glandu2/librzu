#include "UvHandle.h"

template class UvHandle<uv_timer_t>;

template<class HandleType> struct UvHandleInitializerMap {};
template<> struct UvHandleInitializerMap<uv_timer_t> {
	template<typename... Args> static int call(Args... args) { return uv_timer_init(args...); }
};

// template<class HandleType> constexpr int (*InitFunction)(uv_loop_t*, HandleType*);

template<class HandleType> std::vector<std::unique_ptr<HandleType>> UvHandle<HandleType>::freeHandles;
/*
template<typename HandleType> template<typename... Args> UvHandle<HandleType>::UvHandle(Args... args) {
    handle = get();
    UvHandleInitializerMap<HandleType>::call(EventLoop::getLoop(), handle, args...);
}*/

template<typename HandleType> UvHandle<HandleType>::~UvHandle() {
	handle->data = nullptr;
	uv_ref((uv_handle_t*) handle);
	uv_close((uv_handle_t*) handle, &closeCallback);
	handle = nullptr;
}

template<typename HandleType> HandleType* UvHandle<HandleType>::get() {
	if(freeHandles.empty()) {
		return new HandleType;
	} else {
		HandleType* handle = freeHandles.back().release();
		freeHandles.pop_back();
		return handle;
	}
}
