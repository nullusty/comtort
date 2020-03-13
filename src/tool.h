// tool.h
// ------
// captures the state of the currently selected tool for use
// with the SimpleCanvas, which is to be stored within a
// SimpleMenu instance.

#ifndef TOOL_H

#include "toolstate.h"
#include "project.h"
#include "camera.h"

class SimpleTool {
	SimpleToolState* state{ nullptr };
	SimpleProject* project{ nullptr };
	SimpleCamera* camera{ nullptr };
public:
	SimpleTool(SimpleProject* project);

	void SetState(SimpleToolState* newState);
	SimpleToolState* GetState(void);

	SimpleProject* GetProject(void);
	void SetCamera(SimpleCamera* newCamera);
	SimpleCamera* GetCamera(void);
};


#define TOOL_H
#endif