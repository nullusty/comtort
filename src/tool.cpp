#include "tool.h"

// toolstates

// SelectState
// LeftDown -> Select a single node. (NEEDS IMPLEMENTATION)
//	+ CTRL  -> Add a node to the selection.
//  + DRAG  -> Regional select
void Tool::State::SelectState::HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent)
{
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	mMouseDownPos = pTool->mpCamera->ViewTransform(inPos);
	// nodes are drawn with a radius of 8.0f, what is the corresponding radius in view space?
	//  (assumes 1:1 aspect ratio of viewport, fine for now)
	float gRad = 8.0f * pTool->mpCamera->GetViewWidth() / pTool->mpCamera->GetClientWidth();

	// append select
	if (mCtrl) {
		// find which node the user is selecting
		for (auto& pNode : pTool->mpNetwork->GetNodes()) {
			if (intervalContains(pNode->GetX() - gRad, pNode->GetX() + gRad, mMouseDownPos.x)
				& intervalContains(pNode->GetY() - gRad, pNode->GetY() + gRad, mMouseDownPos.y))
			{
				// add it to the list of nodes
				mpNodes.push_back(pNode);
				break;
			}
		}
	}
	// regional select
	else {
			// set drawable region
			mMouseUpPos = mMouseDownPos;
			mDrawable = true;

			// clear currently selected nodes
			mpNodes.clear();
	}
}
void Tool::State::SelectState::HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent)
{
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	mMouseUpPos = pTool->mpCamera->ViewTransform(inPos);

	// singular select
	if (mCtrl) {
		// do nothing
	}
	// regional select
	else {
		// select nodes within region
		for (auto& pNode : pTool->mpNetwork->GetNodes()) {
			if (intervalContains(mMouseDownPos.x, mMouseUpPos.x, pNode->GetX())
				& intervalContains(mMouseDownPos.y, mMouseUpPos.y, pNode->GetY()))
			{
				mpNodes.push_back(pNode);
			}
		}
	}

	// no longer drawable
	mDrawable = false;
}
void Tool::State::SelectState::HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent)
{}
void Tool::State::SelectState::HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent)
{}
void Tool::State::SelectState::HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent)
{
	// update the movable corner of the select region, if we are selecting
	if (mDrawable) {
		vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
		mMouseUpPos = pTool->mpCamera->ViewTransform(inPos);
	}
}
void Tool::State::SelectState::HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent) {}
//  key
void Tool::State::SelectState::HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent)
{	
	wxChar uc = keyEvent.GetUnicodeKey();

	// otherwise
	if (uc != WXK_NONE) {
		// normal character, including control characters in the 1...31 range
		switch (uc) {
		case wxChar('Z'):
			// undo
			if (mCtrl) {
				pTool->mpNetwork->Undo();
			}
			break;
		case wxChar('R'):
			// undo
			if (mCtrl) {
				pTool->mpNetwork->Redo();
			}
			break;
		case WXK_DELETE:
			pTool->mpNetwork->RemoveNodesCmd(mpNodes);
			mpNodes.clear();
			break;
		}
	}
	else {
		// special character
		switch (keyEvent.GetKeyCode())
		{
		case WXK_CONTROL:
			mCtrl = true;
			break;
		case WXK_SHIFT:
			mShift = true;
			break;
		}
	}
}
void Tool::State::SelectState::HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent)
{
	wxChar uc = keyEvent.GetUnicodeKey();
	if (uc != WXK_NONE) {
		// normal character, including control characters in the 1...31 range
	}
	else {
		// special character
		switch (keyEvent.GetKeyCode())
		{
		case WXK_CONTROL:
			mCtrl = false;
		case WXK_SHIFT:
			mShift = false;
		}
	}
}
// rendering
void Tool::State::SelectState::Render(Tool* pTool, wxDC& dc) 
{
	// conditionally draw the select region
	if (mDrawable) {
		// get corners of select region in viewport space
		vec2f ulc = pTool->mpCamera->ViewportTransform(mMouseDownPos);
		vec2f lrc = pTool->mpCamera->ViewportTransform(mMouseUpPos);

		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(wxPen(wxColor(0, 0, 0), 4));
		dc.DrawRectangle(ulc.x, ulc.y, lrc.x - ulc.x, lrc.y - ulc.y);
	}

	// always draw the selected nodes
	for (auto& pNode : mpNodes) {
		// get positions of nodes select region in viewport space
		vec2f inPos{ pNode->GetX(), pNode->GetY() };
		vec2f drawPos = pTool->mpCamera->ViewportTransform(inPos);

		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(wxPen(wxColor(255, 0, 0), 3));
		dc.DrawRectangle(drawPos.x - 8, drawPos.y - 8, 16, 16);
	}
}
// definition
Tool::State::SelectState Tool::State::Select;

// EditState
//  mouse events
void Tool::State::EditState::HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent){
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	vec2f outPos = pTool->mpCamera->ViewTransform(inPos);

	// make a node there
	pTool->mpNetwork->AddNodeCmd(outPos.x, outPos.y);
}
void Tool::State::EditState::HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent){}
void Tool::State::EditState::HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent){
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	vec2f outPos = pTool->mpCamera->ViewTransform(inPos);
	mMouseDownPos.x = outPos.x; mMouseDownPos.y = outPos.y;
	mMouseUpPos.x = outPos.x; mMouseUpPos.y = outPos.y;

	// drawable
	mDrawable = true;
}
void Tool::State::EditState::HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent){
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	vec2f outPos = pTool->mpCamera->ViewTransform(inPos);
	mMouseUpPos.x = outPos.x; mMouseUpPos.y = outPos.y;

	// global radius of rendered nodes
	float gRad = 8.0f * pTool->mpCamera->GetViewWidth() / pTool->mpCamera->GetClientWidth();

	// find which nodes the user selected, if any	
	std::shared_ptr<Node> to{ nullptr }, from{ nullptr };
	for (auto& pNode : pTool->mpNetwork->GetNodes()) {
		if (intervalContains(mMouseDownPos.x - gRad, mMouseDownPos.x + gRad, pNode->GetX())
			& intervalContains(mMouseDownPos.y - gRad, mMouseDownPos.y + gRad, pNode->GetY()))
		{
			from = pNode;
		}
		else if (intervalContains(mMouseUpPos.x - gRad, mMouseUpPos.x + gRad, pNode->GetX())
			& intervalContains(mMouseUpPos.y - gRad, mMouseUpPos.y + gRad, pNode->GetY()))
		{
			to = pNode;
		}
	}
	// create a wire (multiple cases... NEEDS IMPLEMENTATION)
	//pTool->mpNetwork->AddWireC	
	pTool->mpNetwork->AddWireCmd(from, to);

	// no longer drawable
	mDrawable = false;
}
void Tool::State::EditState::HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent){
	if (mDrawable) {
		// project viewport coords -> view coords, capture those
		vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
		vec2f outPos = pTool->mpCamera->ViewTransform(inPos);
		mMouseUpPos.x = outPos.x; mMouseUpPos.y = outPos.y;
	}
}
void Tool::State::EditState::HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent) {}

//  keyboard events
void Tool::State::EditState::HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent)
{
	wxChar uc = keyEvent.GetUnicodeKey();

	// otherwise
	if (uc != WXK_NONE) {
		// normal character, including control characters in the 1...31 range
		switch (uc) {
		case wxChar('Z'):
			// undo
			if (mCtrl) {
				pTool->mpNetwork->Undo();
			}
			break;
		case wxChar('R'):
			// undo
			if (mCtrl) {
				pTool->mpNetwork->Redo();
			}
			break;
		}
	}
	else {
		switch(keyEvent.GetKeyCode()) {
		case WXK_CONTROL:
			mCtrl = true;
			break;
		}
	}
}
void Tool::State::EditState::HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent){}
//  render event
void Tool::State::EditState::Render(Tool* pTool, wxDC& dc) {
	if (mDrawable) {
		// project into viewport
		vec2f inPosFrom{ mMouseDownPos.x, mMouseDownPos.y };
		vec2f outPosFrom = pTool->mpCamera->ViewportTransform(inPosFrom);
		vec2f inPosTo{ mMouseUpPos.x, mMouseUpPos.y };
		vec2f outPosTo = pTool->mpCamera->ViewportTransform(inPosTo);

		dc.SetBrush(*wxBLACK_BRUSH);
		dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
		dc.DrawLine(outPosFrom.x, outPosFrom.y, outPosTo.x, outPosTo.y);
	}
}
//  definition
Tool::State::EditState Tool::State::Edit;

// CameraState
//  mouse
void Tool::State::PanState::HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent) {
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	vec2f outPos = pTool->mpCamera->ViewTransform(inPos);
	mMouseDownPos.x = outPos.x; mMouseDownPos.y = outPos.y;

	mPanning = true;
}
void Tool::State::PanState::HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent) {
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	vec2f outPos = pTool->mpCamera->ViewTransform(inPos);
	mMouseUpPos.x = outPos.x; mMouseUpPos.y = outPos.y;

	// finalize camera transformation
	vec2f trans{ mMouseDownPos.x - mMouseUpPos.x, mMouseDownPos.y - mMouseUpPos.y };
	pTool->mpCamera->TranslateView(trans);

	mPanning = false;
}
void Tool::State::PanState::HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent) {}
void Tool::State::PanState::HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent) {}
void Tool::State::PanState::HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent) {
	if (mPanning) {
		// project viewport coords -> view coords, capture those
		vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
		vec2f outPos = pTool->mpCamera->ViewTransform(inPos);
		mMouseUpPos.x = outPos.x; mMouseUpPos.y = outPos.y;

		// perform translation
		vec2f trans{ mMouseDownPos.x - mMouseUpPos.x, mMouseDownPos.y - mMouseUpPos.y };
		pTool->mpCamera->TranslateView(trans);
	}
}
void Tool::State::PanState::HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent) {
	// zoom delta
	float del = (float)mouseEvent.GetWheelRotation() / (float)mouseEvent.GetWheelDelta();

	// scale view width/height
	float viewWidth = pTool->mpCamera->GetViewWidth();
	pTool->mpCamera->SetViewWidth(viewWidth + del);
	float viewHeight = pTool->mpCamera->GetViewHeight();
	pTool->mpCamera->SetViewHeight(viewHeight + del);
	vec2f scale = { (viewWidth + del) / viewWidth, (viewHeight + del) / viewHeight };;

	// zoom to mouse position
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	vec2f outPos = pTool->mpCamera->ViewTransform(inPos);
	vec2f viewPos = pTool->mpCamera->GetViewPosition();
	vec2f viewPosNew{ scale.x * (viewPos.x - outPos.x) + outPos.x,
				 scale.y * (viewPos.y - outPos.y) + outPos.y };
	pTool->mpCamera->SetViewPosition(viewPosNew);
}
//  key
void Tool::State::PanState::HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent) {
	
}
void Tool::State::PanState::HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent) {

}
//  render
void Tool::State::PanState::Render(Tool* pTool, wxDC& dc) {}
//  definition
Tool::State::PanState Tool::State::Pan;

// MoveState (REDO/UNDO-ABLE?)
//  mouse
void Tool::State::MoveState::HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent) {
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	mMouseDownPos = pTool->mpCamera->ViewTransform(inPos);
	mMouseUpPos = mMouseDownPos;

	// global node radius
	float gRad = 8.0f * pTool->mpCamera->GetViewWidth() / pTool->mpCamera->GetClientWidth();

	// find the node
	for (auto& pNode : pTool->mpNetwork->GetNodes()) {
		if (intervalContains(pNode->GetX() - gRad, pNode->GetX() + gRad, mMouseDownPos.x)
			& intervalContains(pNode->GetY() - gRad, pNode->GetY() + gRad, mMouseDownPos.y)) {
			mpNode = pNode; break;
		}
	}

	// capture original position
	mOldPos.x = mpNode->GetX(); mOldPos.y = mpNode->GetY();
	mMoving = true;
}
void Tool::State::MoveState::HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent) {
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	mMouseUpPos = pTool->mpCamera->ViewTransform(inPos);

	// reset node's position
	mpNode->SetX(mOldPos.x); mpNode->SetY(mOldPos.y);

	// move command
	pTool->mpNetwork->MoveNodeCmd(mpNode, mMouseUpPos);
	
	mMoving = false;
}
void Tool::State::MoveState::HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent) {}
void Tool::State::MoveState::HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent) {}
void Tool::State::MoveState::HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent) {
	if (mMoving) {
		// project viewport coords -> view coords, capture those
		vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
		mMouseUpPos = pTool->mpCamera->ViewTransform(inPos);

		// update the node's position
		mpNode->SetX(mMouseUpPos.x); mpNode->SetY(mMouseUpPos.y);
	}
}
void Tool::State::MoveState::HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent) {}
//  key
void Tool::State::MoveState::HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent) {
	wxChar uc = keyEvent.GetUnicodeKey();

	// otherwise
	if (uc != WXK_NONE) {
		// normal character, including control characters in the 1...31 range
		switch (uc) {
		case wxChar('Z'):
			// undo
			if (mCtrl) {
				pTool->mpNetwork->Undo();
			}
			break;
		case wxChar('R'):
			// undo
			if (mCtrl) {
				pTool->mpNetwork->Redo();
			}
			break;
		}
	}
	else {
		// special character
		switch (keyEvent.GetKeyCode())
		{
		case WXK_CONTROL:
			mCtrl = true;
			break;
		}
	}
}
void Tool::State::MoveState::HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent) {
	wxChar uc = keyEvent.GetUnicodeKey();
	if (uc != WXK_NONE) {
		// normal character, including control characters in the 1...31 range
	}
	else {
		// special character
		switch (keyEvent.GetKeyCode())
		{
		case WXK_CONTROL:
			mCtrl = false;
		}
	}
}
//  render
void Tool::State::MoveState::Render(Tool* pTool, wxDC& dc) {}
//  definition
Tool::State::MoveState Tool::State::Move;

// tool 
Tool::Tool(std::shared_ptr<Network> pNetwork, std::shared_ptr<Camera> pCamera) 
	: mpNetwork(pNetwork), mpCamera(pCamera)
{
	// SelectState is default
	mpState = &State::Select;
}

// state transition interface
void Tool::Select() {
	mpState = &State::Select;
}
void Tool::Edit() {
	mpState = &State::Edit;
}
void Tool::Pan() {
	mpState = &State::Pan;
}
void Tool::Move() {
	mpState = &State::Move;
}

// forwards to state methods for input handling
//  mouse
void Tool::HandleMouseLeftPressed(wxMouseEvent& mouseEvent)
{
	mpState->HandleMouseLeftPressed(this, mouseEvent);
}
void Tool::HandleMouseLeftReleased(wxMouseEvent& mouseEvent)
{
	mpState->HandleMouseLeftReleased(this, mouseEvent);
}
void Tool::HandleMouseRightPressed(wxMouseEvent& mouseEvent)
{
	mpState->HandleMouseRightPressed(this, mouseEvent);
}
void Tool::HandleMouseRightReleased(wxMouseEvent& mouseEvent)
{
	mpState->HandleMouseRightReleased(this, mouseEvent);
}
void Tool::HandleMouseMotion(wxMouseEvent& mouseEvent) {
	mpState->HandleMouseMotion(this, mouseEvent);
}
void Tool::HandleMouseWheel(wxMouseEvent& mouseEvent) {
	mpState->HandleMouseWheel(this, mouseEvent);
}
void Tool::HandleKeyboardKeyPressed(wxKeyEvent& keyEvent)
{
	mpState->HandleKeyboardKeyPressed(this, keyEvent);
}
void Tool::HandleKeyboardKeyReleased(wxKeyEvent& keyEvent)
{
	mpState->HandleKeyboardKeyReleased(this, keyEvent);
}
void Tool::Render(wxDC& dc) {
	mpState->Render(this, dc);
}
