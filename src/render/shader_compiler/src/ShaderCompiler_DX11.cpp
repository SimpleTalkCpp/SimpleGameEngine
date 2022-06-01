#include "ShaderCompiler_DX11.h"

#include <d3d11.h>
#include <d3d11_4.h>
#include <dxgi1_4.h>

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace sge {

void ShaderCompiler_DX11::compile(StrView outPath, ShaderStage shaderStage, StrView srcFilename, StrView entryFunc) {
	TempStringA entryPoint = entryFunc;


	MemMapFile memmap;
	memmap.open(srcFilename);

	auto hlsl = memmap.span();

	UINT flags1 = 0; // D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	UINT flags2 = 0;

#if _DEBUG
	//flags1 |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob>	bytecode;
	ComPtr<ID3DBlob>	errorMsg;

	auto profile = Util::getDxStageProfile(shaderStage);

	auto hr = D3DCompile2(
				hlsl.data(), hlsl.size(), memmap.filename().c_str(),
				nullptr, nullptr,
				entryPoint.c_str(),
				profile, 
				flags1, flags2, 0, nullptr, 0,
				bytecode.ptrForInit(),
				errorMsg.ptrForInit());

	if (FAILED(hr)) {
		throw SGE_ERROR("HRESULT={}\n Error Message: {}", hr, Util::toStrView(errorMsg));
	}

	Directory::create(outPath);

	auto bytecodeSpan = Util::toSpan(bytecode);

	auto outFilename = Fmt("{}/{}.bin", outPath, profile);
	File::writeFile(outFilename, bytecodeSpan, false);

	_reflect(outFilename, bytecodeSpan, profile);
}

void ShaderCompiler_DX11::_reflect(StrView outFilename, ByteSpan bytecode, StrView profile) {
	ComPtr<ID3D11ShaderReflection>	reflect;
	auto hr = D3DReflect(bytecode.data(), bytecode.size(), IID_PPV_ARGS(reflect.ptrForInit()));
	Util::throwIfError(hr);

	D3D11_SHADER_DESC desc;
	hr = reflect->GetDesc(&desc);
	Util::throwIfError(hr);

	ShaderStageInfo outInfo;
	outInfo.profile = profile;

	{
		_reflect_inputs			(outInfo, reflect, desc);
		//_reflect_uniformBuffers	(outInfo, reflect, desc);
		//_reflect_textures		(outInfo, reflect, desc);
		//_reflect_samplers		(outInfo, reflect, desc);
		//_reflect_storageBuffers	(outInfo, reflect, desc);
	}

	{
		auto jsonFilename = Fmt("{}.json", outFilename);
		JsonUtil::writeFile(jsonFilename, outInfo, false);
	}
}

void ShaderCompiler_DX11::_reflect_inputs(ShaderStageInfo& outInfo, ID3D11ShaderReflection* reflect, D3D11_SHADER_DESC& desc) {
	HRESULT hr;

	outInfo.inputs.reserve(desc.InputParameters);

	for (UINT i=0; i<desc.InputParameters; i++) {
		auto& dst = outInfo.inputs.emplace_back();

		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		hr = reflect->GetInputParameterDesc(i, &paramDesc);
		Util::throwIfError(hr);

		VertexSemanticType semanticType;
		semanticType = Util::parseDxSemanticName(StrView_c_str(paramDesc.SemanticName));

		dst.semantic = VertexSemanticUtil::make(semanticType, static_cast<VertexSemanticIndex>(paramDesc.SemanticIndex));

		TempString dataType;

		switch (paramDesc.ComponentType) {
			case D3D_REGISTER_COMPONENT_UINT32:		dataType.append("UInt8");	break;
			case D3D_REGISTER_COMPONENT_SINT32:		dataType.append("Int32");	break;
			case D3D_REGISTER_COMPONENT_FLOAT32:	dataType.append("Float32");	break;
			default: throw SGE_ERROR("unsupported component type {}", paramDesc.ComponentType);
		}

		auto componentCount = BitUtil::count1(paramDesc.Mask);
		if (componentCount < 1 || componentCount > 4) {
			throw SGE_ERROR("invalid componentCount {}", componentCount);
		}

		if (componentCount > 1) {
			FmtTo(dataType, "x{}", componentCount);
		}

		if (!enumTryParse(dst.dataType, dataType)) {
			throw SGE_ERROR("cannot parse dataType enum {}", dataType);
		}
	}
}

}