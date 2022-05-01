#pragma once

namespace sge {

template<class T>
class ComPtr : public NonCopyable {
public:
	ComPtr() = default;

	ComPtr(const ComPtr& r)	{ reset(r._p); }
	ComPtr(ComPtr && r)		{ reset(r._p); r._p = nullptr; }

	void operator=(const ComPtr& r) { if (r._p == _p) return; reset(r._p); }
	void operator=(ComPtr && r)		{ if (r._p == _p) return; reset(r._p); r._p = nullptr; }

	~ComPtr() noexcept { reset(nullptr); }

	T* operator->() noexcept		{ return _p; }
	operator T*() noexcept			{ return _p; }

			T* ptr() noexcept		{ return _p; }
	const	T* ptr() const noexcept	{ return _p; }

	void reset(T* p) {
		if (p == _p) return;
		if (_p) {
			_p->Release();
			_p = nullptr;
		}

		_p = p;
		if (_p) {
			_p->AddRef();
		}
	}

	T** ptrForInit() noexcept { reset(nullptr); return &_p; }

	T* detach() { T* o = _p; _p = nullptr; return o; }
private:
	T* _p = nullptr;
};

}