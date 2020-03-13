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
	  project(project), tool(tool), 
	  current(&buffers[0]), next(&buffers[1])
{
	// setup buffers
	buffers[0].Create(wxSize(200, 200));
	buffers[1].Create(wxSize(200, 200));

	// setup camera
	//  position
	camera.SetPosition(100.0f, 100.0f);
	//  zoom level
	camera.SetZoomLevel(250.0f);
	//  for aspect ratio computation
	camera.SetViewportHeight(200.0f);
	camera.SetViewportWidth(200.0f);
	//  transform
	camera.ResetTransformMatrix();
}

void SimpleCanvas::PaintEvent(wxPaintEvent& event) {
	wxMemoryDC dc;
	dc.SelectObject(*next);
	Render(dc);
	dc.SelectObject(wxNullBitmap);
	wxPaintDC renderDC(this);
	renderDC.DrawBitmap(*current, wxPoint(0, 0));
}

void SimpleCanvas::PaintNow(void) {
	wxMemoryDC dc;
	dc.SelectObject(*next);
	Render(dc);
	dc.SelectObject(wxNullBitmap);
	wxClientDC renderDC(this);
	renderDC.DrawBitmap(*current, wxPoint(0, 0));
}

void SimpleCanvas::Render(wxMemoryDC& dc) {
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
	std::size_t index = 0;
	for (const auto& node : project->GetNodes()) {
		dc.SetBrush(*wxGREY_BRUSH);
		dc.SetPen(wxPen(wxColor(0, 0, 0), 3));
		// transform nodes from global to camera space
		glm::vec2 tPos = camera.ViewportTransformation(glm::vec2(node.x, node.y));
		// transform nodes from camera to pixel (rendering) space
		dc.DrawCircle(wxPoint(std::round(tPos.x), std::round(tPos.y)), 4);

		// draw forward connections
		dc.SetPen(wxPen(wxColor(0, 0, 0), 3));
		for (const auto& forwardNode : node.GetNodes()) {
			dc.DrawLine(wxPoint(node.x, node.y),
				wxPoint(forwardNode->x, forwardNode->y));
		}

		// if selected, draw a box around it (MOVE TO TOOLSTATE RENDERING?)
		if (project->GetSelectedNodes().at(index)) {
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
			dc.DrawRectangle(node.x - 8, node.y - 8, 16, 16);
		}

		index++;
	}

	// draw tool
	tool->GetState()->Render(*tool, dc);

	// swap buffers
	SwapBuffers();
}
void SimpleCanvas::EmptyEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// do nothing, in particular, do not call event.Skip()
}

void SimpleCanvas::SwapBuffers(void)
{
	// swap pointers
	auto temp = current;
	current = next;
	next = temp;
}

SimpleCamera* SimpleCanvas::GetCamera(void) {
	return &camera;
}

// Event handlers, passed to tool
void SimpleCanvas::LeftMouseDown(wxMouseEvent& event)
{
	tool->GetState()->HandleLeftMouseDown(*tool, event);
	PaintNow();
}
void SimpleCanvas::MouseMotion(wxMouseEvent& event)
{
	tool->GetState()->HandleMouseMotion(*tool, event);
	PaintNow();
}
void SimpleCanvas::LeftMouseUp(wxMouseEvent& event)
{
	tool->GetState()->HandleLeftMouseUp(*tool, event);
	PaintNow();
}

void SimpleCanvas::RightMouseDown(wxMouseEvent& event)
{
	/* old code to delete a node by position
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
	*/
}

// declare static event table
wxBEGIN_EVENT_TABLE(SimpleCanvas, wxPanel)
	EVT_PAINT(SimpleCanvas::PaintEvent)
	EVT_ERASE_BACKGROUND(SimpleCanvas::EmptyEraseBackground)
	EVT_LEFT_DOWN(SimpleCanvas::LeftMouseDown)
	EVT_MOTION(SimpleCanvas::MouseMotion)
	EVT_LEFT_UP(SimpleCanvas::LeftMouseUp)
	EVT_RIGHT_DOWN(SimpleCanvas::RightMouseDown)
wxEND_EVENT_TABLE()
