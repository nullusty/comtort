#include "toolstate.h"

#include "tool.h"

// Static State Definitions
SelectState SimpleToolState::select;
EditState SimpleToolState::edit;

// SelectState
void SelectState::HandleLeftMouseDown(SimpleTool& tool, wxMouseEvent& event)
{
	// unselect all nodes
	tool.GetProject()->UnselectNodes();

	// get global coordates of where the user clicks
	glm::vec4 globalPos = tool.GetCamera()->GetInverseMatrix() * glm::vec4((float)event.GetX(), (float)event.GetY(), 0.0f, 1.0f);
		globalPos.x = globalPos.x / globalPos.w;
		globalPos.y = globalPos.y / globalPos.w;
	ulx = globalPos.x;
	uly = globalPos.y;

	// drawable region
	drawable = true;
}
void SelectState::HandleMouseMotion(SimpleTool& tool, wxMouseEvent& event)
{
	// get global coordates of where the user is hovering over
	glm::vec4 globalPos = tool.GetCamera()->GetInverseMatrix() * glm::vec4((float)event.GetX(), (float)event.GetY(), 0.0f, 1.0f);
		globalPos.x = globalPos.x / globalPos.w;
		globalPos.y = globalPos.y / globalPos.w;

	// update region
	lrx = globalPos.x;
	lry = globalPos.y;
}
void SelectState::HandleLeftMouseUp(SimpleTool& tool, wxMouseEvent& event)
{
	// select nodes within region
	std::size_t index = 0;
	for (const auto& node : tool.GetProject()->GetNodes()) {
		// check for containment
		if (((ulx < node.x) & (uly < node.y)) &
			((lrx > node.x) & (lry > node.y))) {
			tool.GetProject()->SelectNodeByIndex(index);
		}
		else {
			tool.GetProject()->UnselectNodeByIndex(index);
		}
		// update index
		index++;
	}

	// no longer drawable region
	drawable = false;
}
void SelectState::Render(SimpleTool& tool, wxMemoryDC& dc)
{
	if (drawable) {
		// get screen coords of where the user is drawing everything
		glm::vec2 topLeft = tool.GetCamera()->ViewportTransformation(glm::vec2(ulx, uly));
		glm::vec2 botRight = tool.GetCamera()->ViewportTransformation(glm::vec2(lrx, lry));
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(wxPen(wxColor(0, 0, 0), 3));
		dc.DrawRectangle(topLeft.x, topLeft.y, botRight.x - topLeft.x, botRight.y - topLeft.y);
	}
}
// EditState
void EditState::HandleLeftMouseDown(SimpleTool& tool, wxMouseEvent& event)
{
	// add node to project
	tool.GetProject()->AddNodeByPosition(event.GetPosition());
}
void EditState::HandleMouseMotion(SimpleTool& tool, wxMouseEvent& event)
{
	// nothing
}
void EditState::HandleLeftMouseUp(SimpleTool& tool, wxMouseEvent& event)
{	
	// nothing
}
void EditState::Render(SimpleTool& tool, wxMemoryDC& dc)
{
	// nothing
}
