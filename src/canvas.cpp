// canvas.cpp
// ----------
// defines the SimpleCanvas class (see canvas.h)
// which provides an interface to project data (see project.h)
// defines the static event table for this interface
// via wxBEGIN_EVENT_TABLE/wxEND_EVENT_TABLE

#include "canvas.h"

#include <cmath> // for std::round

// SimpleCanvas constructor
SimpleCanvas::SimpleCanvas(wxFrame* parent, SimpleProject* project, SimpleTool* tool)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(200, 200)),
	  project(project), tool(tool)
{
	// set up camera
	//  position
	camera.SetPosition(100.0f, 100.0f);
	//  zoom level
	camera.SetZoomLevel(250.0f);
	//  for aspect ratio computation
	camera.SetViewportHeight(200.0f);
	camera.SetViewportWidth(200.0f);
}

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
		// transform nodes from global to camera space
		glm::vec2 tPos = camera.ViewportTransformation(glm::vec2(node.x, node.y));
		// transform nodes from camera to pixel (rendering) space
		int xCoord = std::round(100 * (tPos.x + 1));
		int yCoord = std::round(100 * (tPos.y + 1));
		dc.DrawCircle(wxPoint(xCoord, yCoord), 4);
	}
}

void SimpleCanvas::MouseDown(wxMouseEvent& event)
{
	// switch on SimpleTool's state (ToolState)
	switch(tool->GetState())
	{
	case ToolState::edit:
		// add a node to the project
		project->AddNodeByPosition(event.GetPosition());
		// redraw
		PaintNow();
		break;
	case ToolState::wire:
		// wire two nodes together (needs full implementation)
		break;
	case ToolState::pan:
		// pan the camera (needs full implementation)
		auto oldPos = camera.GetPosition();
		oldPos.x++;
		camera.SetPosition(oldPos.x, oldPos.y);
		// redraw
		PaintNow();
		break;
	}
}

void SimpleCanvas::RightMouseDown(wxMouseEvent& event)
{
	switch (tool->GetState()) {
	case ToolState::edit:
		// delete which node the mouse is over
		auto p = event.GetPosition();
		auto nodes = project->GetNodes();
		for (std::size_t i = 0; i < nodes.size(); ++i) {
			auto node = nodes.at(i);
			// box collision detection
			if ((std::abs(p.x - std::round(node.x)) < 4 * 2)
				& (std::abs(p.y - std::round(node.y)) < 4 * 2))
			{
				project->DeleteNodeByIndex(i);
			}
		}
		// redraw
		PaintNow();
		break;
	}
}

// declare static event table
wxBEGIN_EVENT_TABLE(SimpleCanvas, wxPanel)
	EVT_PAINT(SimpleCanvas::PaintEvent)
	EVT_LEFT_DOWN(SimpleCanvas::MouseDown)
	EVT_RIGHT_DOWN(SimpleCanvas::RightMouseDown)
wxEND_EVENT_TABLE()
