#ifndef TOOL_H

// wxWidget headers
#include "wx/event.h"	// for wxMouseEvent and wxKeyEvent
#include "wx/dc.h"		// for wxDC

// graph headers
#include "network.h"	// for Network
#include "camera.h"		// for Camera

// C++ headers
#include <memory>		// for std::unique_ptr and std::shared_ptr
#include <stack>
#include <tuple>

class ToolState;

class Tool {
	// internal state structure
	//State* mpState{ nullptr };	// current state
	// stack of (state, popEvent) tuples
	//std::stack<std::tuple<State*, wxEventType>> mpStates{};
	//  -> could just give each state a popEvent field...
	friend class ToolState;
	std::stack<ToolState*> mpStates{};

	// objects to manipulate
	std::shared_ptr<Network> mpNetwork{ nullptr };
	std::shared_ptr<Camera> mpCamera{ nullptr };

	// selected entities
	std::vector<std::shared_ptr<Node>> mpSelectedNodes{};
	std::vector<std::shared_ptr<Wire>> mpSelectedWires{};
	bool mDrawSelected{ true };

public:
	Tool(std::shared_ptr<Network> pNetwork,
		 std::shared_ptr<Camera> pCamera);

	// state transition interface
	// pushdown
	void PushState(ToolState* state);
	void PopState();

	// handlers
	void HandleMouse(wxMouseEvent& mouseEvent);

	//  key
	void HandleKeyboardKeyPressed(wxKeyEvent& keyEvent);
	void HandleKeyboardKeyReleased(wxKeyEvent& keyEvent);
	// tool-specific rendering
	void Render(wxDC& dc);

	// selected entities rendering
	void RenderSelected(wxDC& dc);
};

// Abstract Tool State interface
class ToolState {
	wxEventType mPopEventType{ wxEVT_NULL };

public:
	// static state declarations
	class SelectState;
	class EditState;
	class PanState;
	class MoveState;
	// static state instances
	static SelectState Select;
	static EditState Edit;
	static PanState Pan;
	static MoveState Move;

	virtual ~ToolState() {}

	// forwards from tool object
	// input handlers
	//  mouse
	virtual void HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	virtual void HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	virtual void HandleMouseMiddlePressed(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	virtual void HandleMouseMiddleReleased(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	virtual void HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	virtual void HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	virtual void HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	virtual void HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	//  key
	virtual void HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent) = 0;
	virtual void HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent) = 0;
	// rendering
	virtual void Render(Tool* pTool, wxDC& dc) = 0;

	// set PopEventType
	void SetPopEventType(wxEventType eventType);
	wxEventType GetPopEventType() const;
};

// SelectState
class ToolState::SelectState : public ToolState {
	// captured mouse positions
	vec2f mMouseDownPos{ 0.0f, 0.0f };
	vec2f mMouseUpPos{ 0.0f, 0.0f };

	// select region is drawable
	bool mDrawable{ false };

	// modifiers
	bool mShift{ false };
	bool mCtrl{ false };

public:
	//  mouse
	virtual void HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseMiddlePressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseMiddleReleased(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent);
	//  key
	virtual void HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent);
	virtual void HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent);

	// rendering
	virtual void Render(Tool* pTool, wxDC& dc);
};

// EditState
class ToolState::EditState : public ToolState {
	// captured mouse positions
	vec2f mMouseDownPos{ 0.0f, 0.0f };
	vec2f mMouseUpPos{ 0.0f, 0.0f };

	// wire line is drawable
	bool mDrawable{ false };

	// ctrl modifier
	bool mCtrl{ false };
public:
	//  mouse
	virtual void HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseMiddlePressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseMiddleReleased(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent);
	//  key
	virtual void HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent);
	virtual void HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent);
	// rendering
	virtual void Render(Tool* pTool, wxDC& dc);
};

// PanState
class ToolState::PanState : public ToolState {
	vec2f mMouseDownPos{ 0.0f, 0.0f };
	vec2f mMouseUpPos{ 0.0f, 0.0f };

	bool mPanning{ false };
public:
	virtual void HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseMiddlePressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseMiddleReleased(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent);
	//  key
	virtual void HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent);
	virtual void HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent);
	// rendering
	virtual void Render(Tool* pTool, wxDC& dc);
};

// MoveState
class ToolState::MoveState : public ToolState {
	vec2f mMouseDownPos{ 0.0f, 0.0f };
	vec2f mMouseUpPos{ 0.0f, 0.0f };

	// copy of selected nodes positions
	std::vector<vec2f> mOldPos{};

	bool mMoving{ false };
	bool mCtrl{ false };
public:
	virtual void HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseMiddlePressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseMiddleReleased(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent);
	//  key
	virtual void HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent);
	virtual void HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent);
	// rendering
	virtual void Render(Tool* pTool, wxDC& dc);
};

#define TOOL_H
#endif