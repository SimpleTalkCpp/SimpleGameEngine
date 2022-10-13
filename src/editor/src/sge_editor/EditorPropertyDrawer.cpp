#include "EditorPropertyDrawer.h"

namespace sge {

void EditorPropertyDrawer_struct::draw(DrawRequest& req) {
	auto* ed = EditorContext::instance();
	auto* type = req.field->fieldType;

	EditorUI::CollapsingHeader header(req.field->name);

	EditorUI::PushID	pushId(req.field);

	EditorPropertyDrawRequest cr;
	cr.objectType = type;

	cr.objects.reserve(req.objects.size());
	for (auto& o : req.objects) {
		cr.objects.emplace_back(req.field->getValuePtr(o));
	}

	for (auto& c : type->fields()) {
		auto* drawer = ed->getPropertyDrawer(c.fieldType);
		cr.field = &c;
		drawer->draw(cr);
	}
}

void EditorPropertyDrawer_float::draw(DrawRequest& req) {
	bool isMixed = req.isMixedValue<float>();
	auto mv = makeScopedValue(&EditorUI::showMixedValue, isMixed);

	float oldV = req.getFirstObjectValue<float>();
	float newV = oldV;

	if (!EditorUI::DragFloat(req.field->name, &newV)) return;
	if (isMixed) return;

	float delta = newV - oldV;
	for (auto& obj : req.objects) {
		float v = req.field->getValue<float>(obj);
		float nv = v + delta;
		req.field->setValue<float>(obj, nv);
	}
}

}