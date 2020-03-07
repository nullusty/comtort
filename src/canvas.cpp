// canvas.cpp
// ----------
// defines the SimpleCanvas class (see canvas.h)
// which provides an interface to project data (see project.h)
// defines the static event table for this interface
// via wxBEGIN_EVENT_TABLE/wxEND_EVENT_TABLE

#include "canvas.h"

#include <cmath> // for std::round

// SimpleCanvas constructor
SimpleCanvas::SimpleCanvas(wxFrame* parent, SimpleProject* project)
	: wxPanel(parent, -1, wxDefaultPosition, wxSize(200, 200)),
	  project(project)
{}

void SimpleCanvas::PaintEvent(wxPaintEvent& event) {
	wxPaintDC dc(this);
	Render(dc);
}

void SimpleCanvas::PaintNow(void) {
	wxClientDC dc(this);
	Render(dc);
}

void SimpleCanvas::Render(wxDC& dc) {
	// white background
	dc.SetBackgroundMode(wxSOLID);
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();

	// black borders
	wxSize dcSize = dc.GetSize();
	int dcWidth = dcSize.GetWidth();
	int dcHeight = dcSize.GetHeight();
	dc.SetPen(wxPen(wxColor(0, 0, 0), 4));
	dc.DrawLine(wxPoint(0, 0), wxPoint(0, dcHeight));
	dc.DrawLine(wxPoint(0, dcHeight), wxPoint(dcWidth, dcHeight));
	dc.DrawLine(wxPoint(dcWidth, dcHeight), wxPoint(dcWidth, 0));
	dc.DrawLine(wxPoint(dcWidth, 0), wxPoint(0, 0));

	// draw current nodes
	dc.SetBrush(*wxGREY_BRUSH);
	dc.SetPen(wxPen(wxColor(0, 0, 0), 3));
	for (auto&& node : project->GetNodes()) {
		// transform nodes
		int xCoord = std::round(node.xPos);
		int yCoord = std::round(node.yPos);
		dc.DrawCircle(wxPoint(xCoord, yCoord), 4);
	}
}

void SimpleCanvas::MouseDown(wxMouseEvent& event)
{
	// add a node to the project
	project->AddNodeByPosition(event.GetPosition());

	// redraw
	PaintNow();
}

// declare static event table
wxBEGIN_EVENT_TABLE(SimpleCanvas, wxPanel)
	EVT_PAINT(SimpleCanvas::PaintEvent)
	EVT_LEFT_DOWN(SimpleCanvas::MouseDown)
wxEND_EVENT_TABLE()
