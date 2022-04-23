#pragma once

namespace sge {

struct Win32Util {
	Win32Util() = delete;

	static Rect2f toRect2f(const RECT& rc) {
		return Rect2f(	static_cast<float>(rc.left), 
						static_cast<float>(rc.top),
						static_cast<float>(rc.right - rc.left),
						static_cast<float>(rc.bottom - rc.top));
	}

};

}