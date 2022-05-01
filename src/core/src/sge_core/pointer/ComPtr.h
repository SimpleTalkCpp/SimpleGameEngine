#pragma once

namespace sge {

template<class T>
class ComPtr : public NonCopyable {
public:
	ComPtr() = default;

	ComPtr(const ComPtr& r)	{ reset(r._p); }
	ComPtr(ComPtr && r)		{ _p = r._p; r._p = nullptr; }

	void operator=(const ComPtr& r) { reset(r._p); }
	void operator=(ComPtr && r)		{ reset(nullptr); _p = r._p; r._p = nullptr; }

	~ComPtr() noexcept { reset(nullptr); }

		  T* operator->()       noexcept	{ return _p; }
	const T* operator->() const noexcept	{ return _p; }

	operator       T*()       noexcept		{ return _p; }
	operator const T*() const noexcept		{ return _p; }

			T* ptr()       noexcept			{ return _p; }
	const	T* ptr() const noexcept			{ return _p; }

	void reset(T* p) noexcept {
		if (p == _p) return;
		if (_p) {
			_p->Release();
		}
		_p = p;
		if (_p) {
			_p->AddRef();
		}
	}

	T** ptrForInit() noexcept { reset(nullptr); return &_p; }

	T* detach() noexcept { T* o = _p; _p = nullptr; return o; }
private:
	T* _p = nullptr;
};

}