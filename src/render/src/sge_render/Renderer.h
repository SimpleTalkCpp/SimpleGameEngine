#pragma once

#include "Render_Common.h"
#include "RenderContext.h"
#include "shader/Material.h"
#include "textures/Texture.h"

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


	SPtr<RenderContext>		createContext	(RenderContext_CreateDesc&		desc);
	SPtr<RenderGpuBuffer>	createGpuBuffer	(RenderGpuBuffer_CreateDesc&	desc) { return onCreateGpuBuffer(desc); }
	SPtr<Texture2D>			createTexture2D	(Texture2D_CreateDesc&			desc) { return onCreateTexture2D(desc); }
	SPtr<Shader>			createShader	(StrView filename);
	SPtr<Material>			createMaterial	()	{ return onCreateMaterial(); }

	void onShaderDestory(Shader* shader);

	struct StockTextures {
		SPtr<Texture2D>	white;
		SPtr<Texture2D>	black;
		SPtr<Texture2D>	red;
		SPtr<Texture2D>	green;
		SPtr<Texture2D>	blue;
		SPtr<Texture2D>	magenta;
		SPtr<Texture2D>	error;
	};

	StockTextures stockTextures;

	SPtr<Texture2D>	createSolidColorTexture2D(const Color4b& color);

protected:
	virtual SPtr<RenderContext>		onCreateContext		(RenderContext_CreateDesc&		desc) = 0;
	virtual SPtr<RenderGpuBuffer>	onCreateGpuBuffer	(RenderGpuBuffer_CreateDesc&	desc) = 0;
	virtual SPtr<Texture2D>			onCreateTexture2D	(Texture2D_CreateDesc&			desc) = 0;
	virtual SPtr<Shader>			onCreateShader		(StrView filename) = 0;
	virtual SPtr<Material>			onCreateMaterial	() = 0;

	StringMap<Shader*>	_shaders;

	static Renderer*	s_instance;
	RenderAdapterInfo	_adapterInfo;
	bool _vsync : 1;
};

}