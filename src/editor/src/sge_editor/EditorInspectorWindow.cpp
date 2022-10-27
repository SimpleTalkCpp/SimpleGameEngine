#include "EditorInspectorWindow.h"

namespace sge {

void EditorInspectorWindow::draw(RenderRequest& req, Scene& scene) {
	EditorUI::Window win("Inspector", &_active, ImGuiWindowFlags_MenuBar);

	auto* ed = EditorContext::instance();
	auto entities = ed->entitySelection.entities();

	_tempTypeList.clear();
	_tempTypeMap.clear();

	for (auto& id : entities) {
		auto* e = scene.findEntityById(id);
		if (!e) continue;

		for (auto& c : e->components()) {
			if (!c) continue;
			_addToTempList(c);
		}
	}

	for (auto* list : _tempTypeList) {
		drawComponent(req, *list);
	}
}

void EditorInspectorWindow::_addToTempList(Component* c) {
	auto* type = c->getType();
	auto it = _tempTypeMap.find(type);
	
	ObjectList* list = nullptr;

	if (it == _tempTypeMap.end()) {
		list = &_tempTypeMap[type];
		_tempTypeList.emplace_back(list);
	} else {
		list = &_tempTypeMap[type];
	}

	list->emplace_back(c);
}

void EditorInspectorWindow::drawComponent(RenderRequest& req, ObjectList& list) {
	if (list.size() <= 0) return;
	auto* type = list[0]->getType();

	auto* ed =  EditorContext::instance();

	EditorUI::CollapsingHeader header(type->name);

	EditorPropertyDrawRequest drawReq;

	drawReq.objectType = type;
	drawReq.objects.clear();
	drawReq.objects.reserve(list.size());
	for (auto& o : list ) {
		drawReq.objects.emplace_back(o);
	}

	for (auto& f : type->fields()) {
		if (!f.fieldType) continue;

		drawReq.field = &f;

		if (auto* drawer = ed->getPropertyDrawer(f.fieldType)) {
			drawer->draw(drawReq);
		}
	}

//	EditorUI::DragFloat
}

}