// tool.h
// ------
// captures the state of the currently selected tool for use
// with the SimpleCanvas, which is to be stored within a
// SimpleMenu instance.

#ifndef TOOL_H

enum class ToolState {
	edit,
	pan,
	wire
};

class SimpleTool {
	ToolState state{ ToolState::edit };
public:
	SimpleTool();

	void SetState(ToolState newState);
	ToolState GetState(void);
};

#define TOOL_H
#endif