#pragma once

namespace sge {

template<class T>
class SPtr : public NonCopyable {
public:
	SPtr() = default;

	SPtr(T* p) noexcept			{ reset(p); }
	SPtr(SPtr && r) noexcept	{ reset(r._p); r._p = nullptr; }

	void operator=(T* p) noexcept		{ reset(p); }
	void operator=(SPtr && r) noexcept	{ if (r._p == _p) return; reset(r._p); r._p = nullptr; }

	~SPtr() noexcept { reset(nullptr); }

	T* operator->() noexcept		{ return _p; }
	operator T*() noexcept			{ return _p; }

			T* ptr() noexcept		{ return _p; }
	const	T* ptr() const noexcept	{ return _p; }

	void reset(T* p) noexcept {
		static_assert(std::is_base_of<RefCountBase, T>::value, "");

		if (p == _p) return;
		if (_p) {
			auto c = --_p->_refCount;
			if (c <= 0) {
				sge_delete(_p);
			}
			_p = nullptr;
		}

		_p = p;
		if (_p) {
			_p->_refCount++;
		}
	}

	T* detach() { T* o = _p; _p = nullptr; return o; }
private:
	T* _p = nullptr;
};

}