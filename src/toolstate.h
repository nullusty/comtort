#ifndef TOOLSTATE_H

#include "wx/event.h"
#include "wx/dcmemory.h"

class SimpleTool;

class SelectState;
class EditState;

class SimpleToolState {
public:
	static SelectState select;
	static EditState edit;
public:
	virtual ~SimpleToolState() {}
	virtual void HandleLeftMouseDown(SimpleTool& tool, wxMouseEvent& event) = 0;
	virtual void HandleMouseMotion(SimpleTool& tool, wxMouseEvent& event) = 0;
	virtual void HandleLeftMouseUp(SimpleTool& tool, wxMouseEvent& event) = 0;
	virtual void Render(SimpleTool& tool, wxMemoryDC& dc) = 0;
};

class SelectState : public SimpleToolState {
	// select region (rect) coordinates
	//  upper left
	float ulx{ 0.0f }, uly{ 0.0f };
	//  lower right
	float lrx{ 0.0f }, lry{ 0.0f };

	// don't draw if the user hasn't started selecting
	bool drawable{ false };

public:
	virtual void HandleLeftMouseDown(SimpleTool& tool, wxMouseEvent& event);
	virtual void HandleMouseMotion(SimpleTool& tool, wxMouseEvent& event);
	virtual void HandleLeftMouseUp(SimpleTool& tool, wxMouseEvent& event);
	virtual void Render(SimpleTool& tool, wxMemoryDC& dc);
};

class EditState : public SimpleToolState {
public:
	virtual void HandleLeftMouseDown(SimpleTool& tool, wxMouseEvent& event);
	virtual void HandleMouseMotion(SimpleTool& tool, wxMouseEvent& event);
	virtual void HandleLeftMouseUp(SimpleTool& tool, wxMouseEvent& event);
	virtual void Render(SimpleTool& tool, wxMemoryDC& dc);
};

#define TOOLSTATE_H
#endif