#include "CMeshRenderer.h"

namespace sge {

const TypeInfo* CMeshRenderer::s_getType() {
	class TI : public TI_Base {
	public:
		TI() {
		}
	};
	static TI ti;
	return &ti;
}

void CMeshRenderer::onRender(RenderRequest& req) {
	if (!mesh) return;
	if (!material) return;

	auto* tr = entity()->transform();
	req.drawMesh(SGE_LOC, mesh->mesh, material, tr->worldMatrix());
}

}