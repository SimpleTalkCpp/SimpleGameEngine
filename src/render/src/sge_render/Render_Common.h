#pragma once

#include <sge_core.h>

namespace sge {

struct RenderAdapterInfo {
	String	adapterName;
	i64		memorySize = 0;

	bool	multithread = false;
	bool	hasComputeShader = false;
	bool	shaderHasFloat64 = false;
	int		minThreadGroupSize = 1;
};

enum class RenderPrimitiveType {
	None,
	Points,
	Lines,
	Triangles,
};

}