#pragma once

#include "CRenderer.h"
#include <sge_engine/assets/MeshAsset.h>

namespace sge {

class CMeshRenderer : public CRenderer {
	SGE_OBJECT_TYPE(CMeshRenderer, CRenderer)
public:

	SPtr<MeshAsset>		mesh;
	SPtr<Material>		material;

protected:
	virtual void onRender(RenderRequest& req) override;
};

}