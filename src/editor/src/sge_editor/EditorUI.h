#pragma once

#include "EditorContext.h"

namespace sge {

namespace EditorUI {
	static const char* mixedValueFormat = "--";
	static const char* floatFormat = "%0.3f";
	static bool showMixedValue = false;

	inline bool DragFloat(
			const char* label, 
			float* v, 
			float v_speed = 0.1f, 
			float v_min = std::numeric_limits<float>::lowest(),
			float v_max = std::numeric_limits<float>::max(),
			float v_power = 1)
	{
		return ImGui::DragFloat(	label, v, v_speed, v_min, v_max, 
									showMixedValue ? mixedValueFormat : floatFormat, 
									v_power);
	}

	inline float InputFloat(const char* label, float* v) {
		return ImGui::InputFloat(	label, v, 0, 0, 
									showMixedValue ? mixedValueFormat : floatFormat, 
									ImGuiInputTextFlags_EnterReturnsTrue);
	}


	class Window : public NonCopyable {
	public:
		Window(const char* name, bool* p_open = nullptr, ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar) {
			ImGui::Begin(name, p_open, flags);
		}

		~Window() { ImGui::End(); }
	};

	class TreeNode : public NonCopyable {
	public:
		TreeNode(const char* label, ImGuiTreeNodeFlags flags = 0) {
			_isOpen = ImGui::TreeNodeEx(label, flags);
		}

		~TreeNode() { 
			if (_isOpen) ImGui::TreePop();
		}

		bool isOpen() const { return _isOpen; }

	private:
		bool  _isOpen = true;
	};

	class CollapsingHeader : public NonCopyable {
	public:
		CollapsingHeader(const char* label) {
			ImGui::CollapsingHeader(label, &_visiable);
		}

	private:
		bool _visiable = true;
	};

	class PushID : public NonCopyable {
	public:
		PushID(const void* id)	{ ImGui::PushID(id); }
		PushID(int id)			{ ImGui::PushID(id); }
		~PushID() { ImGui::PopID(); }
	};

	inline bool IsItemClicked() { return ImGui::IsItemClicked(); }

	inline bool IsKeyShift()	{ return ImGui::GetIO().KeyShift; }
	inline bool IsKeyAlt()		{ return ImGui::GetIO().KeyAlt; }
	inline bool IsKeyCtrl()		{ return ImGui::GetIO().KeyCtrl; }
}

}