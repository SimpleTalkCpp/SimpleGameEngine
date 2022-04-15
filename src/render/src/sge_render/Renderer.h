#pragma once

#include "Render_Common.h"

namespace sge {

class RenderContext;
struct RenderContext_CreateDesc;

class Renderer : public NonCopyable {
public:
	static Renderer*	current() { return _current; }

	enum class ApiType {
		None,
		DX11,
		OpenGL,
	};

	struct CreateDesc {
		CreateDesc();
		ApiType		apiType;
		bool multithread : 1;
	};

	static Renderer* create(CreateDesc& desc);

	Renderer();
	virtual ~Renderer();

	const RenderAdapterInfo&	adapterInfo() const { return _adapterInfo; }

	virtual RenderContext*	onCreateContext(RenderContext_CreateDesc& desc) = 0;

protected:
	RenderAdapterInfo	_adapterInfo;
	static Renderer*	_current;
};

}