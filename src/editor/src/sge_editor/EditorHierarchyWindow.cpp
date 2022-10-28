#include "EditorHierarchyWindow.h"

namespace sge {

void EditorHierarchyWindow::draw(RenderRequest& req, Scene& scene) {
	EditorUI::Window win("Hierarchy", &_active, ImGuiWindowFlags_MenuBar);

	auto* ed = EditorContext::instance();
	auto& sel = ed->entitySelection;

	{
		DrawRequest dr { req, sel };
		auto* root = scene.rootEntity();
		if (root) {
			drawEntityChildren(dr, root->transform());
		}
	}
}

void EditorHierarchyWindow::drawEntity(DrawRequest& dr, CTransform* tran) {
	auto* entity = tran->entity();
	auto label = Fmt("{} ({})", entity->name(), entity->id());

	ImGuiTreeNodeFlags flags = ImGuiInputTextFlags_None;
	
	size_t childCount = tran->childCount();
	if (childCount <= 0 )
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (dr.sel.has(entity->id())) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	EditorUI::TreeNode node(label.c_str(), flags);

	if (EditorUI::IsItemClicked()) {
		if (EditorUI::IsKeyCtrl()) {
			dr.sel.add(entity->id());
		} else {
			dr.sel.select(entity->id());
		}
	}

	if (node.isOpen()) {
		drawEntityChildren(dr, tran);
	}
}

void EditorHierarchyWindow::drawEntityChildren(DrawRequest& dr, CTransform* tran) {
	size_t n = tran->childCount();
	for (size_t i = 0; i < n; i++) {
		auto* child = tran->getChild(i);
		drawEntity(dr, child);
	}
}



}
