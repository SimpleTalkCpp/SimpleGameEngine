#include "EditorPropertyDrawer.h"

namespace sge {

void EditorPropertyDrawer_float::draw(DrawRequest& req) {
	bool isMixed = req.isMixedValue<float>();
	auto mv = makeScopedValue(&EditorUI::showMixedValue, isMixed);

	float oldV = req.getFirstObjectValue<float>();
	float newV = oldV;

	if (!EditorUI::DragFloat(req.field->name, &newV)) return;
	if (isMixed) return;

	for (auto& obj : req.objects) {
		req.field->setValue<float>(obj, newV);
	}
}

void EditorPropertyDrawer_struct::draw(DrawRequest& parentreq) {
	auto* ed = EditorContext::instance();
	auto* type = parentreq.field->fieldType;

	EditorUI::CollapsingHeader header(parentreq.field->name);

	EditorUI::PushID	pushId(parentreq.field);

	DrawRequest cr;
	parentreq.createChildRequest(cr);

	for (auto& c : type->fields()) {
		auto* drawer = ed->getPropertyDrawer(c.fieldType);
		cr.field = &c;
		drawer->draw(cr);
	}
}


void EditorPropertyDrawer_Vec3f::draw(DrawRequest& req) {
	Vec3f value = req.getFirstObjectValue<Vec3f>();

	EditorUI::CollapsingHeader header(req.field->name);
	EditorUI::PushID	pushId(req.field);

	bool isMixedX = false;
	bool isMixedY = false;
	bool isMixedZ = false;

	if (req.objects.size() > 1) {
		for (auto& obj : req.objects) {
			const auto& tmp = req.field->getValue<Vec3f>(obj);
			if (tmp.x != value.x) isMixedX = true;
			if (tmp.y != value.y) isMixedY = true;
			if (tmp.z != value.z) isMixedZ = true;
		}
	}

	struct Helper {
		static bool drawField(const char* name, float& value, bool isMixed) {
			auto mv = makeScopedValue(&EditorUI::showMixedValue, isMixed);
			float tmpValue = value;
			if (!EditorUI::DragFloat(name, &tmpValue)) return false;
			if (isMixed) return false;
			value = tmpValue;
			return true;
		}
	};

	bool updatedX = Helper::drawField("x", value.x, isMixedX);
	bool updatedY = Helper::drawField("y", value.y, isMixedY);
	bool updatedZ = Helper::drawField("z", value.z, isMixedZ);
	if (!(updatedX || updatedY || updatedZ)) return;

	for (auto& obj : req.objects) {
		auto v = req.field->getValue<Vec3f>(obj);

		if (updatedX) v.x = value.x;
		if (updatedY) v.y = value.y;
		if (updatedZ) v.z = value.z;

		req.field->setValue<Vec3f>(obj, v);
	}
}

}