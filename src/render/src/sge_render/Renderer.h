#pragma once

#include "Render_Common.h"
#include "RenderContext.h"
#include "Shader/Material.h"

namespace sge {

class RenderContext;
struct RenderContext_CreateDesc;

class RenderGpuBuffer;
struct RenderGpuBuffer_CreateDesc;

class Renderer : public NonCopyable {
public:
	static Renderer*	instance() { return s_instance; }

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


	SPtr<RenderContext>		createContext	(RenderContext_CreateDesc& desc)	{ return onCreateContext(desc); }
	SPtr<RenderGpuBuffer>	createGpuBuffer	(RenderGpuBuffer_CreateDesc& desc)	{ return onCreateGpuBuffer(desc); }
	SPtr<Shader>			createShader	(StrView filename);
	SPtr<Material>			createMaterial	()	{ return onCreateMaterial(); }

	void onShaderDestory(Shader* shader);

protected:
	virtual SPtr<RenderContext>		onCreateContext		(RenderContext_CreateDesc&		desc) = 0;
	virtual SPtr<RenderGpuBuffer>	onCreateGpuBuffer	(RenderGpuBuffer_CreateDesc&	desc) = 0;
	virtual SPtr<Shader>			onCreateShader		(StrView filename) = 0;
	virtual SPtr<Material>			onCreateMaterial	() = 0;

	StringMap<Shader*>	_shaders;

	static Renderer*	s_instance;
	RenderAdapterInfo	_adapterInfo;
	bool _vsync : 1;
};

}