#pragma once

namespace sge {

struct Win32Util {
	Win32Util() = delete;

	static void convert(Rect2f& o, const ::RECT& i) {
		o.x = static_cast<float>(i.left);
		o.y = static_cast<float>(i.top);
		o.w = static_cast<float>(i.right  - i.left);
		o.h = static_cast<float>(i.bottom - i.top);
	}

	static void convert(Vec2f& o, const ::POINT& i) {
		o.x = static_cast<float>(i.x);
		o.y = static_cast<float>(i.y);
	}

	static Vec2f  toVec2f	(const ::POINT& i) { Vec2f  o; convert(o, i); return o; }
	static Rect2f toRect2f	(const ::RECT&  i) { Rect2f o; convert(o, i); return o; }

};

}