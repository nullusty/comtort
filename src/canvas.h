// canvas.h
// --------
// declares the SimpleCanvas class, which
// handles paint requests triggered by mouse
// control, and declares its static event table
// via wxDECLARE_EVENT_TABLE();

#ifndef CANVAS_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "tool.h"
#include "camera.h"
#include "project.h"

class SimpleCanvas : public wxPanel
{
public:
	SimpleCanvas(wxFrame* parent, SimpleProject* project, SimpleTool* tool);

	// rendering events
	void PaintEvent(wxPaintEvent& event);
	void PaintNow(void);
	void Render(wxDC& dc);

	// right click menu
	void RightMouseDown(wxMouseEvent& event);
	// add a node
	void MouseDown(wxMouseEvent& event);

private:
	// project reference (MODEL, this is DELEGATE)
	SimpleProject* project{ nullptr };

	// tool reference
	SimpleTool* tool{ nullptr };

	// camera instance
	SimpleCamera camera;

	// declare static event table
	wxDECLARE_EVENT_TABLE();
	// DO NOT DECLARE ANYTHING ELSE HERE
};

#define CANVAS_H
#endif