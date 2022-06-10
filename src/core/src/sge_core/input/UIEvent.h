#pragma once

#include <sge_core/math/Rect2.h>

namespace sge {

enum class UIMouseEventType {
	None,
	Up,
	Down,
	Move,
	Scroll,
};
SGE_ENUM_ALL_OPERATOR(UIMouseEventType)

enum class UIMouseEventButton {
	None,
	Left	= 1 << 0,
	Middle	= 1 << 1,
	Right	= 1 << 2,
	Button4 = 1 << 3,
	Button5 = 1 << 4,
};
SGE_ENUM_ALL_OPERATOR(UIMouseEventButton)

enum class UIEventModifier {
	None,
	Shift	= 1 << 0,
	Ctrl	= 1 << 1,
	Atl		= 1 << 2,
	Cmd		= 1 << 3,
	Fn		= 2 << 4,
};
SGE_ENUM_ALL_OPERATOR(UIEventModifier)

struct UIMouseEvent {
	using Type		= UIMouseEventType;
	using Button	= UIMouseEventButton;
	using Modifier	= UIEventModifier;

	Type		type		= Type::None;
	Button		button		= Button::None;
	Modifier	modifier	= Modifier::None;

	Vec2f		pos{0,0};
	Vec2f		scroll{0,0};
};

}