#pragma once

#include <sge_core/math/Rect2.h>
#include <sge_core/input/UIEvent.h>

namespace sge {

class NativeUIWindow_Base : public NonCopyable {
public:

	struct CreateDesc {
		CreateDesc()
			: titleBar(true)
			, isMainWindow(false)
			, visible(true)
			, resizable(true)
			, closeButton(true)
			, minButton(true)
			, maxButton(true)
			, centerToScreen(true)
			, alwaysOnTop(false)
		{}

		enum class Type {
			None,
			NormalWindow,
			ToolWindow,
			PopupWindow,
		};

		Type		type = Type::NormalWindow;
		Rect2f		rect {10, 10, 800, 600};

		bool		titleBar		: 1;
		bool		isMainWindow	: 1;
		bool		visible			: 1;
		bool		resizable		: 1;
		bool		closeButton		: 1;
		bool		minButton		: 1;
		bool		maxButton		: 1;
		bool		centerToScreen	: 1;
		bool		alwaysOnTop		: 1;
	};

	void create			(CreateDesc& desc)	{ onCreate(desc); }
	void setWindowTitle	(StrView title)		{ onSetWindowTitle(title); }

	void drawNeeded() { onDrawNeeded(); }

	const Rect2f& clientRect() const { return _clientRect; }

	virtual void onCloseButton() {}
	virtual void onActive(bool isActive) {}
	virtual void onDraw() {}

	virtual void onUINativeMouseEvent(UIMouseEvent& ev);
	virtual void onUIMouseEvent(UIMouseEvent& ev) {}

protected:
	virtual void onCreate(CreateDesc& desc) {}
	virtual void onSetWindowTitle(StrView title) {}
	virtual void onClientRectChanged(const Rect2f& rc) { _clientRect = rc; }
	virtual void onDrawNeeded() {}

	Rect2f	_clientRect {0,0,0,0};

	UIMouseEventButton _pressedMouseButtons = UIMouseEventButton::None;
	Vec2f _mousePos{0,0};
};

}