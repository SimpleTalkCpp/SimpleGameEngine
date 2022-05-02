#pragma once

#include "../vertex/Vertex.h"
#include "../buffer/RenderGpuBuffer.h"

namespace sge {

class RenderMesh;
class RenderSubMesh;

enum class RenderCommandType {
	None,
	ClearFrameBuffers,
	SwapBuffers,
	DrawCall,
};

class RenderCommand : NonCopyable {
public:
	using Type = RenderCommandType;

	RenderCommand(Type type) : _type(type) {}

	virtual ~RenderCommand() {}

	Type type() const { return _type; }

#if _DEBUG
	SrcLoc	debugLoc;
#endif

private:
	Type _type = Type::None;
};

class RenderCommand_ClearFrameBuffers : public RenderCommand {
	using Base = RenderCommand;
	using This = RenderCommand_ClearFrameBuffers;
public:
	RenderCommand_ClearFrameBuffers() : Base(Type::ClearFrameBuffers) {}

	This& setColor(const Color4f& color_)	{ color = color_; return *this; }
	This& setDepth(float depth_)			{ depth = depth_; return *this; }
	This& dontClearColor() { color.reset(); return *this; }
	This& dontClearDepth() { depth.reset(); return *this; }

	Opt<Color4f> color = Color4f(1,1,1,1);
	Opt<float>   depth = 0;
};

class RenderCommand_SwapBuffers : public RenderCommand {
	using Base = RenderCommand;
public:
	RenderCommand_SwapBuffers() : Base(Type::SwapBuffers) {}
};

class RenderCommand_DrawCall : public RenderCommand {
	using Base = RenderCommand;
public:
	RenderCommand_DrawCall() : Base(Type::DrawCall) {}

	RenderPrimitiveType		primitive = RenderPrimitiveType::None;
	const VertexLayout*		vertexLayout = nullptr;
	RenderDataType			indexType = RenderDataType::UInt16;

	SPtr<RenderGpuBuffer>	vertexBuffer;
	SPtr<RenderGpuBuffer>	indexBuffer;

	size_t vertexCount = 0;
	size_t indexCount = 0;
};

class RenderCommandBuffer : public NonCopyable {
public:
	RenderCommand_ClearFrameBuffers* clearFrameBuffers() {
		return newCommand<RenderCommand_ClearFrameBuffers>();
	}

	RenderCommand_SwapBuffers* swapBuffers() {
		return newCommand<RenderCommand_SwapBuffers>();
	}

	void drawMesh	(const SrcLoc& debugLoc, const RenderMesh& mesh);
	void drawSubMesh(const SrcLoc& debugLoc, const RenderSubMesh& subMesh);

	void reset();

	Span<RenderCommand*>	commands() { return _commands; }

	template<class CMD>
	CMD* newCommand() {
		auto* buf = _allocator.allocate(sizeof(CMD));
		auto* cmd = new(buf) CMD();
		_commands.emplace_back(cmd);
		return cmd;
	}

private:
	Vector_<RenderCommand*, 64>	_commands;

	LinearAllocator _allocator;
};

}