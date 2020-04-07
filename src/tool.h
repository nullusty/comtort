#ifndef TOOL_H

#include <memory>		// for unique_ptr and shared_ptr

#include "wx/event.h"	// for wxMouseEvent and wxKeyEvent
#include "wx/dc.h"		// for wxDC

#include "network.h"	// for Network
#include "camera.h"		// for Camera

class Tool {
	// internal state structure
	class State;
	State* mpState{ nullptr };

	// objects to manipulate
	std::shared_ptr<Network> mpNetwork{ nullptr };
	std::shared_ptr<Camera> mpCamera{ nullptr };

public:
	Tool(std::shared_ptr<Network> pNetwork,
		 std::shared_ptr<Camera> pCamera);

	// state transition interface
	void Select();
	void Edit();
	void Pan();
	void Move();

	// forwards to state methods for input handling
	//  mouse
	void HandleMouseLeftPressed(wxMouseEvent& mouseEvent);
	void HandleMouseLeftReleased(wxMouseEvent& mouseEvent);
	void HandleMouseRightPressed(wxMouseEvent& mouseEvent);
	void HandleMouseRightReleased(wxMouseEvent& mouseEvent);
	void HandleMouseMotion(wxMouseEvent& mouseEvent);
	void HandleMouseWheel(wxMouseEvent& mouseEvent);
	//  key
	void HandleKeyboardKeyPressed(wxKeyEvent& keyEvent);
	void HandleKeyboardKeyReleased(wxKeyEvent& keyEvent);

	// forward to state method for rendering
	void Render(wxDC& dc);
};

// Abstract Tool State interface
class Tool::State {
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

	~State() {}
	// input handlers
	//  mouse
	virtual void HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	virtual void HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	virtual void HandleMouseRightPressed(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	virtual void HandleMouseRightReleased(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	virtual void HandleMouseMotion(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	virtual void HandleMouseWheel(Tool* pTool, wxMouseEvent& mouseEvent) = 0;
	//  key
	virtual void HandleKeyboardKeyPressed(Tool* pTool, wxKeyEvent& keyEvent) = 0;
	virtual void HandleKeyboardKeyReleased(Tool* pTool, wxKeyEvent& keyEvent) = 0;

	// rendering
	virtual void Render(Tool* pTool, wxDC& dc) = 0;
};

// SelectState
class Tool::State::SelectState : public Tool::State {
	// selected nodes
	std::vector<std::shared_ptr<Node>> mpNodes{};
	// selected wires
	std::vector<std::shared_ptr<Wire>> mpWires{};

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

class Tool::State::EditState : public Tool::State {
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

class Tool::State::PanState : public Tool::State {
	vec2f mMouseDownPos{ 0.0f, 0.0f };
	vec2f mMouseUpPos{ 0.0f, 0.0f };

	bool mPanning{ false };
public:
	virtual void HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent);
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

class Tool::State::MoveState : public Tool::State {
	vec2f mMouseDownPos{ 0.0f, 0.0f };
	vec2f mMouseUpPos{ 0.0f, 0.0f };

	std::shared_ptr<Node> mpNode{ nullptr };
	vec2f mOldPos{ 0.0f, 0.0f };

	bool mCtrl{ false };
	bool mMoving{ false };
public:
	virtual void HandleMouseLeftPressed(Tool* pTool, wxMouseEvent& mouseEvent);
	virtual void HandleMouseLeftReleased(Tool* pTool, wxMouseEvent& mouseEvent);
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