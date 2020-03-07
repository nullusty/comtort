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

#include "project.h"

class SimpleCanvas : public wxPanel
{
public:
	SimpleCanvas(wxFrame* parent, SimpleProject* project);

	// rendering events
	void PaintEvent(wxPaintEvent& event);
	void PaintNow(void);
	void Render(wxDC& dc);

	// add a node
	void MouseDown(wxMouseEvent& event);

private:
	// model
	SimpleProject* project{ nullptr };

	// declare static event table
	wxDECLARE_EVENT_TABLE();
	// DO NOT DECLARE ANYTHING ELSE HERE
};

#define CANVAS_H
#endif