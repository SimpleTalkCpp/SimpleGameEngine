#if 0
Shader {
	Properties {
	}
	
	Pass {
		// Queue	"Transparent"
//		Cull		None

		DepthTest	LessEqual

//		DepthTest	Always
//		DepthWrite	false

		BlendRGB 	Add One OneMinusSrcAlpha
		BlendAlpha	Add One OneMinusSrcAlpha
		
		VsFunc		vs_main
		PsFunc		ps_main
	}
}
#endif

struct VertexIn {
	float2 positionOS : POSITION;
};

struct PixelIn {
	float4 positionHCS : SV_POSITION;
	float3 positionWS  : TEXCOORD10;
	float2 uv		: TEXCOORD0;
	float4 color  	: COLOR;
	float3 normal 	: NORMAL;
};

float4x4	sge_matrix_model;
float4x4	sge_matrix_view;
float4x4	sge_matrix_proj;
float4x4	sge_matrix_mvp;

float3		sge_camera_pos;
float3		sge_light_pos;
float3		sge_light_dir;
float		sge_light_power;
float3		sge_light_color;

Texture2D    terrainHeightMap;
SamplerState terrainHeightMap_Sampler;

int 	patchCellsPerRow;
int2	patchIndex;
float2	patchSize;
float3  terrainPos;
float2  terrainSize;
float   terrainHeight;

PixelIn vs_main(VertexIn i) {
	PixelIn o;

//	float2 patchPos = patchIndex * patchSize;
	float2 patchPos = patchIndex * (patchSize + 1); // debug
	float2 vertexPos2D = float2(patchPos + i.positionOS);

	float2 uv = (i.positionOS + patchIndex * patchSize) / terrainSize;

	float y = terrainHeight * terrainHeightMap.SampleLevel(terrainHeightMap_Sampler, uv, 0).r;

	float4 positionOS = float4(	vertexPos2D.x, y, vertexPos2D.y, 1);

	positionOS.xyz += terrainPos;

	float4 posWS = mul(sge_matrix_model, positionOS);
	o.positionWS  = posWS.xyz / posWS.w;
	o.positionHCS = mul(sge_matrix_mvp,   positionOS);
	
	o.uv = uv;
	o.color	 = 1;
	o.normal = 0;
	return o;
}

struct Surface {
	float3 positionWS;
	float3 normal;	
	float3 color;
	float3 ambient;
	float3 diffuse;
	float3 specular;
	float  shininess;
};

float3 Color_Linear_to_sRGB(float3 x) { return x < 0.0031308 ? 12.92 * x : 1.13005 * sqrt(x - 0.00228) - 0.13448 * x + 0.005719; }
float3 Color_sRGB_to_Linear(float3 x) { return x < 0.04045 ? x / 12.92 : -7.43605 * x - 31.24297 * sqrt(-0.53792 * x + 1.279924) + 35.34864; }

float3 lighting_blinn_phong(Surface s) {
	float3 normal   = normalize(s.normal);
	float3 lightDir = s.positionWS - sge_light_pos;
	float  lightDistance = length(sge_light_dir);
	lightDir = normalize(lightDir);

	float lambertian = max(dot(normal, -sge_light_dir), 0);
	float specularPower = 0;

	if (lambertian > 0) {
		float3 viewDir = normalize(sge_camera_pos - s.positionWS);

		if (1) { // blinn-phong
			float3 halfDir = normalize(viewDir - lightDir);
			float specAngle = saturate(dot(halfDir, normal));
			specularPower = pow(specAngle, s.shininess);

		} else { // phong
			float3 reflectDir = reflect(-lightDir, normal);
			float specAngle = max(dot(reflectDir, viewDir), 0);
			specularPower = pow(specAngle, s.shininess / 4.0);
		}
	}

	float3 outLightColor = sge_light_color * sge_light_power / (lightDistance * lightDistance);

	float3 outAmbient  = s.ambient;
	float3 outDiffuse  = s.diffuse  * lambertian    * outLightColor;
	float3 outSpecular = s.specular * specularPower * outLightColor;

	float3 outColor = (outAmbient + outDiffuse + outSpecular) * s.color;
	return outColor;
}

float4 ps_main(PixelIn i) : SV_TARGET
{
	return float4(i.uv.x, i.uv.y, 0, 1);

//	return float4(i.positionHCS.w * 0.05, 0, 0, 1);
//	return float4(i.normal, 1);
//	return i.color * test_color;
//	return float4(i.uv.x, i.uv.y, 0, 1);

	Surface s;
	s.positionWS = i.positionWS;
	s.normal     = i.normal;
	s.color	     = float3(1, 1, 1);
	s.specular   = float3(0.8, 0.8, 0.8);
	s.ambient    = float3(0.2, 0.2, 0.2);
	s.diffuse	 = float3(1, 1, 1);
	s.shininess	 = 1;

	float3 color = lighting_blinn_phong(s);
	return float4(Color_Linear_to_sRGB(color), 1);
}
