#include "page.h"

// wxWidgets headers
#include "wx/button.h"		// for wxButton
#include "wx/sizer.h"		// for wxBoxSizer
#include "wx/event.h"		// for Bind

Page::Page(wxNotebook* parent)
	: wxWindow(parent, wxID_ANY)
{
	// create graph objects
	//  network
	mpNetwork = std::make_shared<Network>();
	//  camera
	rect viewRect{ 10.0f, 10.0f };
	vec2f viewPos{ -5.0f, -5.0f };
	float viewAngle{ 0.0f };
	rect clientRect{ 200.0f, 200.0f };
	mpCamera = std::make_shared<Camera>(viewPos, viewRect, viewAngle, clientRect);

	// basic sizer
	wxBoxSizer* pSizer = new wxBoxSizer(wxVERTICAL);
	// create tool buttons
	//  sizer
	wxBoxSizer* pButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* pSelectButton = new wxButton(this, wxID_ANY, wxT("select"));
	pButtonSizer->Add(pSelectButton, 0, 0, 0);
	wxButton* pEditButton = new wxButton(this, wxID_ANY, wxT("edit"));
	pButtonSizer->Add(pEditButton, 0, 0, 0);
	wxButton* pPanButton = new wxButton(this, wxID_ANY, wxT("pan"));
	pButtonSizer->Add(pPanButton, 0, 0, 0);
	wxButton* pMoveButton = new wxButton(this, wxID_ANY, wxT("move"));
	pButtonSizer->Add(pMoveButton, 0, 0, 0);
	pSizer->Add(pButtonSizer, 0, 0, 0);
	// create canvasp
	mpCanvas = new Canvas(this, mpCamera, mpNetwork);
	// add to sizer
	pSizer->Add(mpCanvas, 0, 0, 0);
	// bind network paint event (could be static)
	mpCanvas->Bind(wxEVT_PAINT, &Canvas::PaintEvent, mpCanvas);
	// bind event handlers (could be static)
	mpCanvas->Bind(wxEVT_KEY_DOWN, &Canvas::HandleKeyDown, mpCanvas);
	mpCanvas->Bind(wxEVT_KEY_UP, &Canvas::HandleKeyUp, mpCanvas);
	// bind button events to methods of Canvas (requires dynamic binding)
	pSelectButton->Bind(wxEVT_BUTTON, &Canvas::HandleSelectButton, mpCanvas);
	pEditButton->Bind(wxEVT_BUTTON, &Canvas::HandleEditButton, mpCanvas);
	pPanButton->Bind(wxEVT_BUTTON, &Canvas::HandlePanButton, mpCanvas);
	pMoveButton->Bind(wxEVT_BUTTON, &Canvas::HandleMoveButton, mpCanvas);
	// set focus on canvas when it is hovered overp
	mpCanvas->Bind(wxEVT_ENTER_WINDOW, &Canvas::HandleMouseEnter, mpCanvas);

	mpCanvas->SetFocus();

	//  project
	mpProject = std::make_shared<Project>();

	// set window sizer
	SetSizer(pSizer);

	// center window in display
	Centre();
}