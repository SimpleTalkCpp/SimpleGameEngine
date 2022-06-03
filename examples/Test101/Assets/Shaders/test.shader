#if 0
Shader {
	Properties {
		Float	test  = 0.5
		Vec4f	test2 = {0,0,0,1}
		
		[DisplayName="Color Test"]
		Color4f	color = {1,1,1,1}
	}
	
	Pass {
		// Queue	"Transparent"
		// Cull		None

		// BlendRGB 	Add One OneMinusSrcAlpha
		// BlendAlpha	Add One OneMinusSrcAlpha

		// DepthTest	Always
		// DepthWrite	false
		
		VsFunc		vs_main
		PsFunc		ps_main
	}
}
#endif

struct VertexIn {
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelIn {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4x4	SGE_MVP;

float  test_float;
float4 test_color;

PixelIn vs_main(VertexIn i) {
    PixelIn o;
    o.position = i.position;
	o.position.y += test_float;
	
    o.color    = i.color;
    return o;
}

float4 ps_main(PixelIn i) : SV_TARGET
{
//	return float4(1,0,0,1);
    return i.color * test_color;
}
