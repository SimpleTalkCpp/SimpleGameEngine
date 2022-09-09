#if 0
Shader {
	Properties {
	}
	
	Pass {
		// Queue	"Transparent"
		Cull		None

		DepthTest	Always
//		DepthWrite	false

		BlendRGB 	Add One OneMinusSrcAlpha
		BlendAlpha	Add One OneMinusSrcAlpha
		
		VsFunc		vs_main
		PsFunc		ps_main
	}
}
#endif

struct VertexIn {
	float2 pos : POSITION;
	float4 col : COLOR0;
	float2 uv  : TEXCOORD0;
};

struct PixelIn {
	float4 pos 	: SV_POSITION;
	float4 col 	: COLOR0;
	float2 uv	: TEXCOORD0;
};

float4x4 ProjectionMatrix; 

Texture2D    texture0;
SamplerState sampler0;

PixelIn vs_main(VertexIn i) {
	PixelIn o;
	o.pos = mul(ProjectionMatrix, float4(i.pos.xy, 0.f, 1.f));
	o.col = i.col;
	o.uv  = i.uv;
	return o;
}

float4 ps_main(PixelIn i) : SV_TARGET {
	float4 o = i.col * texture0.Sample(sampler0, i.uv).r;
	return o;
}
