#include "canvas.h"

Canvas::Canvas(wxWindow* pParent, std::shared_ptr<Camera> pCamera,
	std::shared_ptr<Network> pNetwork)
	: wxPanel(pParent, wxID_ANY, wxDefaultPosition,
		wxSize(400, 400))
{
	// instantiate network and camera ptrs
	mpNetwork = pNetwork;
	mpCamera = pCamera;

	// create tool
	mpTool = std::make_shared<Tool>(pNetwork, pCamera);

	// create bitmap buffers
	mBuffers[0].Create(wxSize(400, 400));
	mBuffers[1].Create(wxSize(400, 400));

	// set buffer pointers
	mpCurrentBuffer = &mBuffers[0];
	mpNextBuffer = &mBuffers[1];
}

void Canvas::SwapBuffers() {
	auto pTemp = mpCurrentBuffer;
	mpCurrentBuffer = mpNextBuffer;
	mpNextBuffer = pTemp;
}

void Canvas::SetCamera(std::shared_ptr<Camera> pCamera) {
	mpCamera = pCamera;
}
void Canvas::SetNetwork(std::shared_ptr<Network> pNetwork) {
	mpNetwork = pNetwork;
}

void Canvas::PaintEvent(wxPaintEvent& event) {
	wxMemoryDC dc;
	dc.SelectObject(*mpNextBuffer);
	Render(dc);
	dc.SelectObject(wxNullBitmap);
	wxPaintDC paintDC(this);
	paintDC.DrawBitmap(*mpCurrentBuffer, wxPoint(0, 0));
}

void Canvas::PaintNow() {
	wxMemoryDC dc;
	dc.SelectObject(*mpNextBuffer);
	Render(dc);
	dc.SelectObject(wxNullBitmap);
	wxClientDC clientDC(this);
	clientDC.DrawBitmap(*mpCurrentBuffer, wxPoint(0, 0));
}

void Canvas::Render(wxDC& dc) {
	// draw background
	DrawBackground(dc);

	// draw network
	DrawNetwork(dc);

	// draw tool
	DrawTool(dc);

	// swap buffers
	SwapBuffers();
}

void Canvas::DrawBackground(wxDC& dc) {
	// draws a white background
	dc.SetBackgroundMode(wxSOLID);
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
}

void Canvas::DrawNetwork(wxDC& dc) {
	// currently unoptimal - node positions are transformed several times this way instead of just once.

	// draw wires
	for (auto& pWire : mpNetwork->GetWires()) {
		vec2f inPosFrom{ pWire->GetFromX(), pWire->GetFromY() };
		vec2f inPosTo{ pWire->GetToX(), pWire->GetToY() };
		vec2f drawPosFrom = mpCamera->ViewportTransform(inPosFrom);
		vec2f drawPosTo = mpCamera->ViewportTransform(inPosTo);
		dc.SetBrush(*wxBLACK_BRUSH);
		dc.SetPen(wxPen(wxColor(0, 0, 0), 3));
		dc.DrawLine(wxPoint(drawPosFrom.x, drawPosFrom.y), wxPoint(drawPosTo.x, drawPosTo.y));
	}

	// draw nodes on top of wires
	for (auto& pNode : mpNetwork->GetNodes()) {
		vec2f inPos{ pNode->GetX(), pNode->GetY() };
		vec2f drawPos = mpCamera->ViewportTransform(inPos);
		dc.SetBrush(*wxGREY_BRUSH);
		dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
		dc.DrawCircle(wxPoint(drawPos.x, drawPos.y), 8);
	}
}

void Canvas::DrawTool(wxDC& dc) {
	mpTool->Render(dc);
}

void Canvas::HandleMouseLeftPressed(wxMouseEvent& evt) {
	mpTool->HandleMouseLeftPressed(evt);
	PaintNow();
}

void Canvas::HandleMouseLeftReleased(wxMouseEvent& evt) {
	mpTool->HandleMouseLeftReleased(evt);
	PaintNow();
}

void Canvas::HandleMouseRightPressed(wxMouseEvent& evt) {
	mpTool->HandleMouseRightPressed(evt);
	PaintNow();
}

void Canvas::HandleMouseRightReleased(wxMouseEvent& evt) {
	mpTool->HandleMouseRightReleased(evt);
	PaintNow();
}

void Canvas::HandleMouseMotion(wxMouseEvent& evt) {
	mpTool->HandleMouseMotion(evt);

	PaintNow();
}

void Canvas::HandleMouseWheel(wxMouseEvent& evt) {
	mpTool->HandleMouseWheel(evt);
	PaintNow();
}

void Canvas::HandleKeyDown(wxKeyEvent& evt) {
	mpTool->HandleKeyboardKeyPressed(evt);
	PaintNow();
}

void Canvas::HandleKeyUp(wxKeyEvent& evt) {
	mpTool->HandleKeyboardKeyReleased(evt);
	PaintNow();
}

void Canvas::HandleSelectButton(wxCommandEvent& evt) {
	mpTool->Select();
	SetFocus();
}

void Canvas::HandleEditButton(wxCommandEvent& evt) {
	mpTool->Edit();
	SetFocus();
}

void Canvas::HandlePanButton(wxCommandEvent& evt) {
	mpTool->Pan();
	SetFocus();
}

void Canvas::HandleMoveButton(wxCommandEvent& evt) {
	mpTool->Move();
	SetFocus();
}

void Canvas::HandleMouseEnter(wxMouseEvent& evt) {
	SetFocus();
}