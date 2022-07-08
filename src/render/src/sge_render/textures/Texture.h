#pragma once

#include "Image.h"

namespace sge {

#define TextureFilter_ENUM_LIST(E) \
	E(None,) \
	E(Point,)      /* 1 texels on closest mipmap level */ \
	E(Linear,)     /* 1 texels on 2 mipmap levels */ \
	E(Bilinear,)   /* 4 texels on closest mipmap level */  \
	E(Trilinear,)  /* 4 texels on 2 mipmap levels */\
	E(Anisotropic,) \
//----
SGE_ENUM_CLASS(TextureFilter, u8)

#define TextureWrap_ENUM_LIST(E) \
	E(None,) \
	E(Repeat,) \
	E(Clamp,) \
	E(Mirror,) \
	E(MirrorOnce,) \
//----
SGE_ENUM_CLASS(TextureWrap, u8)

struct SamplerState {
public:
	using Filter	= TextureFilter;
	using Wrap		= TextureWrap;

	Filter	filter	= Filter::Bilinear;
	Wrap	wrapU	= Wrap::Repeat;
	Wrap	wrapV	= Wrap::Repeat;
	Wrap	wrapW	= Wrap::Repeat;

	float	minLOD	= 0;
	float	maxLOD	= Math::inf<float>();
};

struct Texture_CreateDesc {
public:
	String		name;
	ColorType	colorType = ColorType::RGBAb;
	int			mipmapCount = 1;
};

class Texture : public RefCountBase {
public:
	using CreateDesc = Texture_CreateDesc;
	using DataType   = RenderDataType;

	DataType type() const { return _type; }

protected:
	Texture() = default;
	DataType _type = DataType::None;
};

struct Texture2D_CreateDesc : public Texture_CreateDesc {
	Vec2i			size {0,0};
	SamplerState	samplerState;
	Image			imageToUpload;
};

class Texture2D : public Texture {
public:
	using CreateDesc	= Texture2D_CreateDesc;
	
	ColorType	colorType	() const	{ return _colorType; }
	int			mipmapCount	() const	{ return _mipmapCount; }
	int			width		() const	{ return _size.x; }
	int			height		() const	{ return _size.y; }
	Vec2i		size		() const	{ return _size; }

protected:
	Texture2D(CreateDesc& desc); // please create from Renderer::createTexture2D()

	ColorType	_colorType = ColorType::None;
	int			_mipmapCount = 0;
	Vec2i		_size {0,0};
};

} // namespace