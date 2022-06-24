#include "ShaderCompiler_DX11.h"

#include <d3d11.h>
#include <d3d11_4.h>
#include <dxgi1_4.h>

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace sge {

void ShaderCompiler_DX11::compile(StrView outPath, ShaderStageMask shaderStage, StrView srcFilename, StrView entryFunc) {
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
	File::writeFileIfChanged(outFilename, bytecodeSpan, false);

	_reflect(outFilename, bytecodeSpan, shaderStage, profile);
}

void ShaderCompiler_DX11::_reflect(StrView outFilename, ByteSpan bytecode, ShaderStageMask stage, StrView profile) {
	ComPtr<ID3D11ShaderReflection>	reflect;
	auto hr = D3DReflect(bytecode.data(), bytecode.size(), IID_PPV_ARGS(reflect.ptrForInit()));
	Util::throwIfError(hr);

	D3D11_SHADER_DESC desc;
	hr = reflect->GetDesc(&desc);
	Util::throwIfError(hr);

	ShaderStageInfo outInfo;
	outInfo.profile = profile;
	outInfo.stage   = stage;

	{
		_reflect_inputs			(outInfo, reflect, desc);
		_reflect_constBuffers	(outInfo, reflect, desc);
		_reflect_textures		(outInfo, reflect, desc);
		_reflect_samplers		(outInfo, reflect, desc);
	}

	{
		auto jsonFilename = Fmt("{}.json", outFilename);
		JsonUtil::writeFileIfChanged(jsonFilename, outInfo, false);
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

		TempString  semantic = enumStr(dst.semantic);
		if (!semantic.size()) {
			throw SGE_ERROR("unsupported sematic name {}", paramDesc.SemanticName);
		}

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

void ShaderCompiler_DX11::_reflect_constBuffers(ShaderStageInfo& outInfo, ID3D11ShaderReflection* reflect, D3D11_SHADER_DESC& desc) {
	HRESULT hr;

	outInfo.constBuffers.reserve(desc.BoundResources);

	for (UINT i=0; i<desc.BoundResources; i++) {
		D3D11_SHADER_INPUT_BIND_DESC resDesc;
		hr = reflect->GetResourceBindingDesc(i, &resDesc);
		Util::throwIfError(hr);

		if (resDesc.Type != D3D_SIT_CBUFFER) continue;

		auto& outCB = outInfo.constBuffers.emplace_back();

		D3D11_SHADER_BUFFER_DESC bufDesc;
		auto cb = reflect->GetConstantBufferByName(resDesc.Name);
		hr = cb->GetDesc(&bufDesc);
		Util::throwIfError(hr);

		outCB.name		= bufDesc.Name;
		outCB.bindPoint	= static_cast<i16>(resDesc.BindPoint);
		outCB.bindCount	= static_cast<i16>(resDesc.BindCount);
		outCB.dataSize	= bufDesc.Size;

		{
			outCB.variables.reserve(bufDesc.Variables);
			for (UINT j=0; j<bufDesc.Variables; j++) {
				auto cbv = cb->GetVariableByIndex(j);
				D3D11_SHADER_VARIABLE_DESC varDesc;
				hr = cbv->GetDesc(&varDesc);
				Util::throwIfError(hr);

				D3D11_SHADER_TYPE_DESC varType;
				hr = cbv->GetType()->GetDesc(&varType);
				Util::throwIfError(hr);

				if (0 == (varDesc.uFlags & D3D_SVF_USED)) continue;

				auto& outVar = outCB.variables.emplace_back();
				outVar.name = varDesc.Name;
				outVar.offset = varDesc.StartOffset;
				//------------------------------
				TempString dataType;
				switch (varType.Type) {
					case D3D_SVT_BOOL:	dataType.append("Bool");	break;
					case D3D_SVT_INT:	dataType.append("Int32");	break;
					case D3D_SVT_UINT:	dataType.append("UInt32");	break;
					case D3D_SVT_UINT8:	dataType.append("UInt8");	break;
					case D3D_SVT_FLOAT: dataType.append("Float32");	break;
					case D3D_SVT_DOUBLE:dataType.append("Float64");	break;
					default: throw SGE_ERROR("unsupported type {}", varType.Type);
				}

				switch (varType.Class) {
					case D3D_SVC_SCALAR: break;
					case D3D_SVC_VECTOR:
						FmtTo(dataType, "x{}",	varType.Columns);
						break;
					case D3D_SVC_MATRIX_COLUMNS:
						FmtTo(dataType, "_{}x{}", varType.Rows, varType.Columns); 
						outVar.rowMajor = false;
						break;
					case D3D_SVC_MATRIX_ROWS:
						FmtTo(dataType, "_{}x{}", varType.Rows, varType.Columns); 
						outVar.rowMajor = true;
						break;
					default: throw SGE_ERROR("unsupported Class {}", varType.Class);
				}

				if (!enumTryParse(outVar.dataType, dataType)) {
					throw SGE_ERROR("cannot parse dataType {}", dataType);
				}

				if (outVar.dataType == DataType::None) {
					throw SGE_ERROR("dataType is None");
				}
			}
		}
	}

}

void ShaderCompiler_DX11::_reflect_textures(ShaderStageInfo& outInfo, ID3D11ShaderReflection* reflect, D3D11_SHADER_DESC& desc) {
	HRESULT hr;

	outInfo.textures.reserve(desc.BoundResources);
	for (UINT i=0; i<desc.BoundResources; i++) {
		D3D11_SHADER_INPUT_BIND_DESC resDesc;
		hr = reflect->GetResourceBindingDesc(i, &resDesc);
		Util::throwIfError(hr);

		if (resDesc.Type != D3D_SIT_TEXTURE) continue;

		auto& outTex = outInfo.textures.emplace_back();
		outTex.name			= resDesc.Name;
		outTex.bindPoint	= static_cast<i16>(resDesc.BindPoint);
		outTex.bindCount	= static_cast<i16>(resDesc.BindCount);

		switch (resDesc.Dimension) {
			case D3D_SRV_DIMENSION_TEXTURE1D:		outTex.dataType = DataType::Texture1D;   break;
			case D3D_SRV_DIMENSION_TEXTURE2D:		outTex.dataType = DataType::Texture2D;   break;
			case D3D_SRV_DIMENSION_TEXTURE3D:		outTex.dataType = DataType::Texture3D;   break;
			case D3D_SRV_DIMENSION_TEXTURECUBE:		outTex.dataType = DataType::TextureCube; break;
		//----
			case D3D_SRV_DIMENSION_TEXTURE1DARRAY:	outTex.dataType = DataType::Texture1DArray;   break;
			case D3D_SRV_DIMENSION_TEXTURE2DARRAY:	outTex.dataType = DataType::Texture2DArray;   break;
			case D3D_SRV_DIMENSION_TEXTURECUBEARRAY:outTex.dataType = DataType::TextureCubeArray; break;
		//----
			default: throw SGE_ERROR("unsupported texture dimension {}", resDesc.Dimension);
		}
	}
}

void ShaderCompiler_DX11::_reflect_samplers(ShaderStageInfo& outInfo, ID3D11ShaderReflection* reflect, D3D11_SHADER_DESC& desc) {
	HRESULT hr;
	outInfo.samplers.reserve(desc.BoundResources);
	for (UINT i=0; i<desc.BoundResources; i++) {
		D3D11_SHADER_INPUT_BIND_DESC resDesc;
		hr = reflect->GetResourceBindingDesc(i, &resDesc);
		Util::throwIfError(hr);

		if (resDesc.Type != D3D_SIT_SAMPLER) continue;

		auto& outSampler = outInfo.samplers.emplace_back();
		outSampler.name			= resDesc.Name;
		outSampler.bindPoint	= static_cast<i16>(resDesc.BindPoint);
		outSampler.bindCount	= static_cast<i16>(resDesc.BindCount);
	}

}

}