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
	void Render(wxMemoryDC& dc);
	void EmptyEraseBackground(wxEraseEvent& event);

	// swap buffers
	void SwapBuffers(void);

	// right click menu
	void RightMouseDown(wxMouseEvent& event);
	// mouse events, passed to tool instance
	void LeftMouseDown(wxMouseEvent& event);
	void MouseMotion(wxMouseEvent& event);
	void LeftMouseUp(wxMouseEvent& event);

	SimpleCamera* GetCamera(void);

private:
	// project reference (MODEL, this is DELEGATE)
	SimpleProject* project{ nullptr };

	// buffers (double buffered)
	wxBitmap buffers[2];
	//  pointers
	wxBitmap* current{ nullptr };
	wxBitmap* next{ nullptr };

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