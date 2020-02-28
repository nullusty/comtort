// canvas.cpp
// ----------
// defines the SimpleCanvas class (see canvas.h)
// which provides an interface to project data (see project.h)
// defines the static event table for this interface
// via wxBEGIN_EVENT_TABLE/wxEND_EVENT_TABLE

#include "canvas.h"

// SimpleCanvas constructor
SimpleCanvas::SimpleCanvas(wxFrame* parent)
	: wxPanel(parent, -1, wxDefaultPosition, wxSize(200, 200))
{
	// bind mousedown to event in parent window
}

// called by wxWidgets when the panel needs to 
// be redrawn - can be triggered by calling
// Refresh()/Update()
void SimpleCanvas::paintEvent(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	render(dc);
}

// called by a clientDC to paint on the panel
// at any time
void SimpleCanvas::paintNow()
{
	wxClientDC dc(this);
	render(dc);
}

void SimpleCanvas::render(wxDC& dc)
{
	// draw a circle
	//dc.SetBrush(*wxGREEN_BRUSH);
	//dc.SetPen(wxPen(wxColor(255, 0, 0), 5));
	//dc.DrawCircle(wxPoint(100, 100), 5);
}

void SimpleCanvas::mouseDown(wxMouseEvent& event)
{
	wxClientDC dc(this);

	// draw a circle there
	dc.SetBrush(*wxBLUE_BRUSH);
	dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
	dc.DrawCircle(event.GetPosition(), 12);
}

// declare static event table
wxBEGIN_EVENT_TABLE(SimpleCanvas, wxPanel)
	EVT_PAINT(SimpleCanvas::paintEvent)
	EVT_LEFT_DOWN(SimpleCanvas::mouseDown)
wxEND_EVENT_TABLE()
