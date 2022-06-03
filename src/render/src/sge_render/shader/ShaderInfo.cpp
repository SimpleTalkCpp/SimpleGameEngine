#include "ShaderInfo.h"

namespace sge {

void ShaderInfo::clear() {
	props.clear();
	passes.clear();
}

void ShaderStageInfo::ConstBuffer::setParamToBuffer(
	Span<u8> dstBuf,
	StrView propName,
	const ShaderPropValueConstPtr& valuePtr
) const
{
	using ValuePtr = ShaderPropValueConstPtr;
	using PropType = ShaderPropType;

	struct Req : public NonCopyable {
		Span<u8> dstBuf;
		const Variable* variable = nullptr;
		const ValuePtr* valuePtr = nullptr;;
	};

	Req req;
	req.valuePtr = &valuePtr;
	req.dstBuf = dstBuf;
	
	for (auto& it : variables) {
		if (it.name == propName) {
			req.variable = &it;
			break;
		}
	}

	if (!req.variable) return;

	#if 0
	template<class T>
	struct Helper {
		static T* getPtr(Req& req) {
			auto offset = variable->offset;
			auto end = offset + sizeof(T);
			if (end >= dstBuf.size()) throw SGE_ERROR("param out of range");
			return reinterpret_cast<T*>(dstBuf.data() + offset);
		}

		void cannotSet(Req& req) {
			default: throw SGE_ERROR("cannot set value from {} to {}", valuePtr.type, variable->dataType);
		}
	};

	struct Helper_Tuple4f : public Helper<Tuple4f> {
		bool setValue(Req& req) {
			switch (req.valuePtr->type) {
				case PropType::Color4f: {
					auto* dst = getPtr(dstBuf, variable);
					*dst = valuePtr.asValue<Color4f>().toTuple();
					return true;
				} break;
			}

			cannotSet(req)
		}
	};

	switch (variable.dataType) {
		case DataType::Float32x4: Helper_Tuple4f::setValue(req); break;
	}
	#endif
}

}