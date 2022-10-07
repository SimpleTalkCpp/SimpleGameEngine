#include "EditorHierarchyWindow.h"

namespace sge {

void EditorHierarchyWindow::draw(Scene& scene, RenderRequest& req) {
	EditorUI::Window win("Hierarchy", &_active, ImGuiWindowFlags_MenuBar);

	auto* ed = EditorContext::instance();
	auto& sel = ed->entitySelection;

	TempString	tmp;
	for (auto& e : scene.entities()) {
		tmp.clear();
		FmtTo(tmp, "{} ({})", e->name(), e->id());

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;

		if (sel.has(e->id())) {
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		EditorUI::TreeNode node(tmp, flags);

		if (EditorUI::IsItemClicked()) {
			if (EditorUI::IsKeyCtrl()) {
				sel.add(e->id());
			} else {
				sel.select(e->id());
			}
		}
	}

}

}
