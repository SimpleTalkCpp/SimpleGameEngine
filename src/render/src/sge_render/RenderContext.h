#pragma once

namespace sge {

struct RenderContext_CreateDesc {
	NativeUIWindow*	window = nullptr;
};

class RenderContext : public NonCopyable {
public:
	using CreateDesc = RenderContext_CreateDesc;

	static RenderContext*	create(CreateDesc& desc);

	RenderContext(CreateDesc& desc);
	virtual ~RenderContext() = default;
};


}