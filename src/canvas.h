// canvas.h
// --------
// declares the SimpleCanvas class, which
// handles paint requests triggered by mouse
// control, and declares its static event table
// via wxDECLARE_EVENT_TABLE();

#ifndef CANVAS_H

#include "wx/wx.h"

class SimpleCanvas : public wxPanel
{
public:
	SimpleCanvas(wxFrame* parent);

	// painting events
	void paintEvent(wxPaintEvent& event);
	void paintNow();

	void render(wxDC& dc);

	void mouseDown(wxMouseEvent& event);

private:

	// declare static event table
	wxDECLARE_EVENT_TABLE();
};

#define CANVAS_H
#endif