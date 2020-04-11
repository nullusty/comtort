#include "tool.h"

// toolstates
void ToolState::SetPopEventType(wxEventType eventType) {
	mPopEventType = eventType;
}
wxEventType ToolState::GetPopEventType() const {
	return mPopEventType;
}

// SelectState
// LeftDown -> Select a single node. (NEEDS IMPLEMENTATION)
//	+ CTRL  -> Add a node to the selection.
//  + DRAG  -> Regional select
void ToolState::SelectState::HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent)
{
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	mMouseDownPos = pTool->mpCamera->ViewTransform(inPos);

	// nodes are drawn in viewport space with a radius of 8.0f, what is the corresponding radius in view space?
	//  (assumes 1:1 aspect ratio of viewport, fine for now)
	float gRad = 8.0f * pTool->mpCamera->GetViewWidth() / pTool->mpCamera->GetClientWidth();

	// move state transition
	for (auto& pNode : pTool->mpSelectedNodes) {
		if (intervalContains(pNode->GetX() - gRad, pNode->GetX() + gRad, mMouseDownPos.x)
			& intervalContains(pNode->GetY() - gRad, pNode->GetY() + gRad, mMouseDownPos.y))
		{
			// go into movestate, call the same handler, and when finished restore this state.
			pTool->PushState(&ToolState::Move);
			pTool->mpStates.top()->SetPopEventType(wxEVT_LEFT_UP);
			pTool->mpStates.top()->HandleMouseLeftPressed(pTool, mouseEvent);
			// StateToPush, wxEventType, std::function<void(pTool, wxMouseEvent)>
			// Transition(State*, wxEventType, std::function)
			return;
		}
	}

	// append select
	if (mCtrl) {
		// find which node the user is selecting
		for (auto& pNode : pTool->mpNetwork->GetNodes()) {
			if (intervalContains(pNode->GetX() - gRad, pNode->GetX() + gRad, mMouseDownPos.x)
				& intervalContains(pNode->GetY() - gRad, pNode->GetY() + gRad, mMouseDownPos.y))
			{
				// add it to the list of nodes
				pTool->mpSelectedNodes.push_back(pNode);
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
		pTool->mpSelectedNodes.clear();
	}
}
void ToolState::SelectState::HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent)
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
				pTool->mpSelectedNodes.push_back(pNode);
			}
		}
	}

	// no longer drawable
	mDrawable = false;
}
void ToolState::SelectState::HandleMouseMiddlePressed(Tool* pTool, wxMouseEvent& mouseEvent)
{
	pTool->PushState(&ToolState::Pan);
	pTool->mpStates.top()->SetPopEventType(wxEVT_MIDDLE_UP);
	pTool->mpStates.top()->HandleMouseLeftPressed(pTool, mouseEvent);
}
void ToolState::SelectState::HandleMouseMiddleReleased(Tool* pTool, wxMouseEvent& mouseEvent)
{}
void ToolState::SelectState::HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent)
{}
void ToolState::SelectState::HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent)
{}
void ToolState::SelectState::HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent)
{
	// update the movable corner of the select region, if we are selecting
	if (mDrawable) {
		vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
		mMouseUpPos = pTool->mpCamera->ViewTransform(inPos);
	}
}
void ToolState::SelectState::HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent) 
{
	// How to do this correctly with the stack? Not generalizable to instantiated states.
	// -> How can the PopEventType be "not wheel"?
	ToolState::Pan.HandleMouseWheel(pTool, mouseEvent);
}
//  key
void ToolState::SelectState::HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent)
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
			pTool->mpNetwork->RemoveNodesCmd(pTool->mpSelectedNodes);
			pTool->mpSelectedNodes.clear();
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
void ToolState::SelectState::HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent)
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
void ToolState::SelectState::Render(Tool* pTool, wxDC& dc) 
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

}
// definition
ToolState::SelectState ToolState::Select;

// EditState
//  mouse events
void ToolState::EditState::HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent){
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	vec2f outPos = pTool->mpCamera->ViewTransform(inPos);

	// make a node there
	pTool->mpNetwork->AddNodeCmd(outPos.x, outPos.y);
}
void ToolState::EditState::HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent){}
void ToolState::EditState::HandleMouseMiddlePressed(Tool* pTool, wxMouseEvent& mouseEvent)
{}
void ToolState::EditState::HandleMouseMiddleReleased(Tool* pTool, wxMouseEvent& mouseEvent)
{}
void ToolState::EditState::HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent){
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	vec2f outPos = pTool->mpCamera->ViewTransform(inPos);
	mMouseDownPos.x = outPos.x; mMouseDownPos.y = outPos.y;
	mMouseUpPos.x = outPos.x; mMouseUpPos.y = outPos.y;

	// drawable
	mDrawable = true;
}
void ToolState::EditState::HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent){
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
void ToolState::EditState::HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent){
	if (mDrawable) {
		// project viewport coords -> view coords, capture those
		vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
		vec2f outPos = pTool->mpCamera->ViewTransform(inPos);
		mMouseUpPos.x = outPos.x; mMouseUpPos.y = outPos.y;
	}
}
void ToolState::EditState::HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent) {}
//  keyboard events
void ToolState::EditState::HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent)
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
void ToolState::EditState::HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent){}
//  render event
void ToolState::EditState::Render(Tool* pTool, wxDC& dc) {
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
ToolState::EditState ToolState::Edit;

// PanState
//  mouse
void ToolState::PanState::HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent) {
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	vec2f outPos = pTool->mpCamera->ViewTransform(inPos);
	mMouseDownPos.x = outPos.x; mMouseDownPos.y = outPos.y;

	mPanning = true;
}
void ToolState::PanState::HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent) {
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	vec2f outPos = pTool->mpCamera->ViewTransform(inPos);
	mMouseUpPos.x = outPos.x; mMouseUpPos.y = outPos.y;

	// finalize camera transformation
	vec2f trans{ mMouseDownPos.x - mMouseUpPos.x, mMouseDownPos.y - mMouseUpPos.y };
	pTool->mpCamera->TranslateView(trans);

	mPanning = false;
}
void ToolState::PanState::HandleMouseMiddlePressed(Tool* pTool, wxMouseEvent& mouseEvent) {}
void ToolState::PanState::HandleMouseMiddleReleased(Tool* pTool, wxMouseEvent& mouseEvent) {}
void ToolState::PanState::HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent) {}
void ToolState::PanState::HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent) {}
void ToolState::PanState::HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent) {
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
void ToolState::PanState::HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent) {
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
void ToolState::PanState::HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent) {
	
}
void ToolState::PanState::HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent) {

}
//  render
void ToolState::PanState::Render(Tool* pTool, wxDC& dc) {}
//  definition
ToolState::PanState ToolState::Pan;

// MoveState
//  mouse
void ToolState::MoveState::HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent) {
	// project viewport coords -> view coords, capture those
	vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
	mMouseDownPos = pTool->mpCamera->ViewTransform(inPos);
	mMouseUpPos = mMouseDownPos;

	// global node radius
	float gRad = 8.0f * pTool->mpCamera->GetViewWidth() / pTool->mpCamera->GetClientWidth();

	// make sure user clicks on a single one  of the selected nodes
	for (auto& pNode : pTool->mpSelectedNodes) {
		if (intervalContains(pNode->GetX() - gRad, pNode->GetX() + gRad, mMouseDownPos.x)
			& intervalContains(pNode->GetY() - gRad, pNode->GetY() + gRad, mMouseDownPos.y)) {
			mMoving = true;
		}
	}

	// save current positions of selected  nodes
	if (mMoving) {
		mOldPos.clear();
		for (auto& pNode : pTool->mpSelectedNodes) {
			mOldPos.push_back(pNode->GetPosition());
		}
	}
}
void ToolState::MoveState::HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent) {
	if (mMoving) {
		// project viewport coords -> view coords, capture those
		vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
		mMouseUpPos = pTool->mpCamera->ViewTransform(inPos);

		// compute transformation
		vec2f trans{ mMouseUpPos.x - mMouseDownPos.x, mMouseUpPos.y - mMouseDownPos.y };

		// reset node's position
		for (std::size_t i = 0; i < pTool->mpSelectedNodes.size(); ++i) {
			pTool->mpSelectedNodes.at(i)->SetPosition(mOldPos.at(i));
		}

		// move command
		pTool->mpNetwork->MoveNodesCmd(pTool->mpSelectedNodes, trans);

		mMoving = false;
	}
}
void ToolState::MoveState::HandleMouseMiddlePressed(Tool* pTool, wxMouseEvent& mouseEvent) {}
void ToolState::MoveState::HandleMouseMiddleReleased(Tool* pTool, wxMouseEvent& mouseEvent) {}
void ToolState::MoveState::HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent) {}
void ToolState::MoveState::HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent) {}
void ToolState::MoveState::HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent) {
	if (mMoving) {
		// project viewport coords -> view coords, capture those
		vec2f inPos{ mouseEvent.GetX(), mouseEvent.GetY() };
		mMouseUpPos = pTool->mpCamera->ViewTransform(inPos);

		// compute current transformation
		vec2f trans{ mMouseUpPos.x - mMouseDownPos.x, mMouseUpPos.y - mMouseDownPos.y };

		// update the node's position
		for (std::size_t i = 0; i < pTool->mpSelectedNodes.size(); ++i) {
			pTool->mpSelectedNodes.at(i)->SetX(mOldPos.at(i).x + trans.x); 
			pTool->mpSelectedNodes.at(i)->SetY(mOldPos.at(i).y + trans.y); 
		}
	}
}
void ToolState::MoveState::HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent) {}
//  key
void ToolState::MoveState::HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent) {
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
void ToolState::MoveState::HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent) {
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
void ToolState::MoveState::Render(Tool* pTool, wxDC& dc) {}
//  definition
ToolState::MoveState ToolState::Move;

// tool 
Tool::Tool(std::shared_ptr<Network> pNetwork, std::shared_ptr<Camera> pCamera) 
	: mpNetwork(pNetwork), mpCamera(pCamera)
{
	// SelectState is default
	//mpState = &State::Select;
	mpStates.push(&ToolState::Select);
}

// state transition interface
void Tool::PushState(ToolState* newState) {
	mpStates.push(newState);
}
void Tool::PopState() {
	mpStates.pop();
}

// this is where the stack of states are managed.
//  if an event of mpState->GetPopEventType() is 
//  observed, the corresponding input handler is 
//  called followed by popping the state stack.
void Tool::HandleMouse(wxMouseEvent& mouseEvent) 
{
	if (!mpStates.empty()) {
		wxEventType eventType = mouseEvent.GetEventType();
		if (eventType == wxEVT_LEFT_DOWN) {
			mpStates.top()->HandleMouseLeftPressed(this, mouseEvent);
			if (mpStates.top()->GetPopEventType() == wxEVT_LEFT_DOWN) {
				PopState();
			}
		}
		else if (eventType == wxEVT_LEFT_UP) {
			mpStates.top()->HandleMouseLeftReleased(this, mouseEvent);
			if (mpStates.top()->GetPopEventType() == wxEVT_LEFT_UP) {
				PopState();
			}
		}
		else if (eventType == wxEVT_MIDDLE_DOWN) {
			mpStates.top()->HandleMouseMiddlePressed(this, mouseEvent);
			if (mpStates.top()->GetPopEventType() == wxEVT_MIDDLE_DOWN) {
				PopState();
			}
		}
		else if (eventType == wxEVT_MIDDLE_UP) {
			mpStates.top()->HandleMouseMiddleReleased(this, mouseEvent);
			if (mpStates.top()->GetPopEventType() == wxEVT_MIDDLE_UP) {
				PopState();
			}
		}
		else if (eventType == wxEVT_RIGHT_DOWN) {
			mpStates.top()->HandleMouseRightPressed(this, mouseEvent);
			if (mpStates.top()->GetPopEventType() == wxEVT_RIGHT_DOWN) {
				PopState();
			}
		}
		else if (eventType == wxEVT_RIGHT_UP) {
			mpStates.top()->HandleMouseRightReleased(this, mouseEvent);
			if (mpStates.top()->GetPopEventType() == wxEVT_RIGHT_UP) {
				PopState();
			}
		}
		else if (eventType == wxEVT_MOTION) {
			mpStates.top()->HandleMouseMotion(this, mouseEvent);
			if (mpStates.top()->GetPopEventType() == wxEVT_MOTION) {
				PopState();
			}
		}
		else if (eventType == wxEVT_MOUSEWHEEL) {
			mpStates.top()->HandleMouseWheel(this, mouseEvent);
			if (mpStates.top()->GetPopEventType() == wxEVT_MOUSEWHEEL) {
				PopState();
			}
		}
	}
}
void Tool::HandleKeyboardKeyPressed(wxKeyEvent& keyEvent) {
	mpStates.top()->HandleKeyboardKeyPressed(this, keyEvent);
}
void Tool::HandleKeyboardKeyReleased(wxKeyEvent& keyEvent) {
	mpStates.top()->HandleKeyboardKeyReleased(this, keyEvent);
}

void Tool::Render(wxDC& dc) {
	// always draw selected wires and nodes
	RenderSelected(dc);
	// toolstate-specific rendering
	mpStates.top()->Render(this, dc);
}

void Tool::RenderSelected(wxDC& dc) {

	// draw selected nodes
	for (auto& pNode : mpSelectedNodes) {
		// get positions of nodes select region in viewport space
		vec2f inPos{ pNode->GetX(), pNode->GetY() };
		vec2f drawPos = mpCamera->ViewportTransform(inPos);

		// draw a rectangle
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(wxPen(wxColor(255, 0, 0), 3));
		dc.DrawRectangle(drawPos.x - 8, drawPos.y - 8, 16, 16);
	}
}
