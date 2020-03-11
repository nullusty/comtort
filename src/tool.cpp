#include "tool.h"

SimpleTool::SimpleTool() {}

void SimpleTool::SetState(ToolState newState) {
	state = newState;
}

ToolState SimpleTool::GetState(void) {
	return state;
}