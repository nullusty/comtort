#ifndef CANVAS_H

// wxWidget headers
#include "wx/panel.h"		// for wxPanel
#include "wx/event.h"		// for wxMouseEvent and wxKeyEvent
#include "wx/dcmemory.h"	// for wxMemoryDC
#include "wx/dcclient.h"	// for wxPaintDC and wxClientDC
#include "wx/bitmap.h"		// for wxBitmap

// graph headers
#include "network.h"		// for Network
#include "project.h"		// for Project
#include "tool.h"			// for Tool
#include "camera.h"			// for Camera

// Canvas class
class Canvas : public wxPanel {
	// drawing buffers (for double buffering)
	wxBitmap mBuffers[2];
	wxBitmap* mpCurrentBuffer;
	wxBitmap* mpNextBuffer;

	// camera instance
	std::shared_ptr<Camera> mpCamera{ nullptr };
	// network instance
	std::shared_ptr<Network> mpNetwork{ nullptr };
	// tool instance
	std::shared_ptr<Tool> mpTool{ nullptr };

public:
	Canvas(wxWindow* pParent, std::shared_ptr<Camera> pCamera,
		std::shared_ptr<Network> pNetwork);
	
	void SwapBuffers();

	void SetCamera(std::shared_ptr<Camera> pCamera);
	void SetNetwork(std::shared_ptr<Network> pNetwork);

	void PaintEvent(wxPaintEvent& event);
	void PaintNow();

	void Render(wxDC& dc);
	void DrawBackground(wxDC& dc);
	void DrawNetwork(wxDC& dc);
	void DrawTool(wxDC& dc);

	void HandleMouseLeftPressed(wxMouseEvent& evt);
	void HandleMouseLeftReleased(wxMouseEvent& evt);
	void HandleMouseRightPressed(wxMouseEvent& evt);
	void HandleMouseRightReleased(wxMouseEvent& evt);
	void HandleMouseMotion(wxMouseEvent& evt);
	void HandleMouseWheel(wxMouseEvent& evt);
	void HandleKeyDown(wxKeyEvent& evt);
	void HandleKeyUp(wxKeyEvent& evt);

	void HandleMouseEnter(wxMouseEvent& evt);

	void HandleSelectButton(wxCommandEvent& evt);
	void HandleEditButton(wxCommandEvent& evt);
	void HandlePanButton(wxCommandEvent& evt);
	void HandleMoveButton(wxCommandEvent& evt);
};

#define CANVAS_H
#endif