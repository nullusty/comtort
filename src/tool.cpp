#include "tool.h"

SimpleTool::SimpleTool(SimpleProject* project)
	: project(project), camera(nullptr), state(&SimpleToolState::edit)
{}

void SimpleTool::SetState(SimpleToolState* newState) {
	state = newState;
}

SimpleToolState* SimpleTool::GetState(void) {
	return state;
}

SimpleProject* SimpleTool::GetProject(void) {
	return project;
}

void SimpleTool::SetCamera(SimpleCamera* newCamera) {
	camera = newCamera;
}

SimpleCamera* SimpleTool::GetCamera(void) {
	return camera;
}
