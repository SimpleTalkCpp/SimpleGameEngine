#if 0
Shader {
	Properties {
	}
	
	Pass {
		// Queue	"Transparent"
//		Cull		None

//		DepthTest	LessEqual

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
	float4 positionOS : POSITION;
	float4 color : COLOR;
};

struct PixelIn {
	float4 positionHCS : SV_POSITION;
	float4 color : COLOR;
};

float4x4	sge_matrix_mvp;

PixelIn vs_main(VertexIn i) {
	PixelIn o;
	o.positionHCS = mul(sge_matrix_mvp,   i.positionOS);
	o.color	 = i.color;
	return o;
}

float4 ps_main(PixelIn i) : SV_TARGET {
	return i.color;
}
