#pragma once

#include <sge_core/math/Rect2.h>

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
		Rect2f		rect {10, 10, 640, 480};

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

	void create(CreateDesc& desc) { onCreate(desc); }

protected:
	virtual void onCreate(CreateDesc& desc) {}
	virtual void onCloseButton() {}
	virtual void onActive(bool isActive) {}
};

}