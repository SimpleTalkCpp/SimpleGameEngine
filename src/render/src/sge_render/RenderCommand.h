#pragma once

#include "RenderDrawCall.h"

namespace sge {

class RenderMesh;

enum class RenderCommandType {
	None,
	SwapBuffer,
	DrawCall,
};

class RenderCommand : NonCopyable {
public:
	using Type = RenderCommandType;

	RenderCommand(Type type) : _type(type) {}

	virtual ~RenderCommand() {}

	Type type() const { return _type; }

private:
	Type _type = Type::None;
};

class RenderCommand_SwapBuffer : public RenderCommand {
	using Base = RenderCommand;
public:
	RenderCommand_SwapBuffer() : Base(Type::SwapBuffer) {}
};

class RenderCommand_DrawCall : public RenderCommand {
	using Base = RenderCommand;
public:
	RenderCommand_DrawCall() : Base(Type::DrawCall) {}

	RenderDrawCall drawcall;
};

class RenderCommandBuffer : public NonCopyable {
public:
	void drawMesh(RenderMesh& mesh);
	void swapBuffer();

	void clearCommands() {
		_allocator.clear();

		for (auto* cmd : commands) {
			cmd->~RenderCommand();
		}
		commands.clear();
	}

	Vector_<RenderCommand*, 64>	commands;

private:
	template<class CMD>
	CMD* newCommand() {
		auto* cmd = reinterpret_cast<CMD*>(_allocator.allocate(sizeof(CMD)));
		commands.emplace_back(cmd);
		return cmd;
	}

	LinearAllocator _allocator;
};

}