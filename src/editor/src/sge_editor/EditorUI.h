#pragma once

#include "EditorContext.h"

namespace sge {

namespace EditorUI {

	class Window : public NonCopyable {
	public:
		Window(StrView name, bool* p_open = nullptr, ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar) {
			ImGui::Begin(TempString(name).c_str(), p_open, flags);
		}

		~Window() { ImGui::End(); }
	};

	class TreeNode : public NonCopyable {
	public:
		TreeNode(StrView label, ImGuiTreeNodeFlags flags = 0) {
			_isOpen = ImGui::TreeNodeEx(TempString(label).c_str(), flags);
		}

		~TreeNode() { ImGui::TreePop(); }

		bool isOpen() const { return _isOpen; }

	private:
		bool  _isOpen;
	};

	inline bool IsItemClicked() { return ImGui::IsItemClicked(); }

	inline bool IsKeyShift()	{ return ImGui::GetIO().KeyShift; }
	inline bool IsKeyAlt()		{ return ImGui::GetIO().KeyAlt; }
	inline bool IsKeyCtrl()		{ return ImGui::GetIO().KeyCtrl; }
}

}