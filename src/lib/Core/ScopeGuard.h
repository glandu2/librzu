#pragma once

#include <functional>

template<typename Callable> class ScopeGuard {
public:
	ScopeGuard(Callable&& undo_func) : enabled(true), f(std::forward<Callable>(undo_func)) {}
	ScopeGuard(ScopeGuard&& other) : enabled(other.enabled), f(std::move(other.f)) { other.enabled = false; }

	~ScopeGuard() {
		if(enabled)
			f();
	}

	void dismiss() { enabled = false; }

private:
	ScopeGuard(const ScopeGuard&);
	ScopeGuard& operator=(const ScopeGuard&);

	bool enabled;
	Callable f;
};

template<typename F> ScopeGuard<F> make_scope_guard(F&& f) {
	return ScopeGuard<F>(std::forward<F>(f));
};

#define guard_on_exit(name, x) \
	auto name = make_scope_guard(x); \
	(void) name;

