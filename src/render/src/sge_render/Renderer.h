#pragma once

#include "Render_Common.h"

namespace sge {

class RenderContext;
struct RenderContext_CreateDesc;

class RenderGpuBuffer;
struct RenderGpuBuffer_CreateDesc;

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

	bool vsync() const		{ return _vsync; }


	RenderContext*		createContext(RenderContext_CreateDesc& desc)		{ return onCreateContext(desc); }
	RenderGpuBuffer*	createGpuBuffer(RenderGpuBuffer_CreateDesc& desc)	{ return onCreateGpuBuffer(desc); }

protected:
	virtual RenderContext*		onCreateContext		(RenderContext_CreateDesc&		desc) = 0;
	virtual RenderGpuBuffer*	onCreateGpuBuffer	(RenderGpuBuffer_CreateDesc&	desc) = 0;

	static Renderer*	_current;
	RenderAdapterInfo	_adapterInfo;
	bool _vsync : 1;
};

}