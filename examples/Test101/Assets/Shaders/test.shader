Shader {
	Properties {
		Float	test  = {0,0,0,1}
		Vec4f	test2 = 0.5
		
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

struct VIn {
	float4 position : POSITION;
	float4 color : COLOR
}

struct VOut {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4x4	SGE_MVP;

float x;
float b;
float c;

VOut vs_main(VIn in)
{
    VOut output;

    output.position = position;
    output.color = color;

    return output;
}

float4 ps_main(VOut) : SV_TARGET
{
    return color;
}
