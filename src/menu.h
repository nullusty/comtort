// menu.h
// ------
// declares the SimpleMenu class (public wxFrame), 
// the main window of comtort, including declaring 
// SimpleMenu's static event table via wxDECLARE_EVENT_TABLE

#ifndef MENU_H

// For compilers that don't support precompilation,
// include "wx/wx.h"
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "tool.h"		// for SimpleTool
#include "canvas.h"		// for SimpleCanvas
#include "project.h"	// for SimpleProject

class SimpleMenu : public wxFrame
{
public:
	// constructor
	SimpleMenu(const wxString& title);

	// menu event handlers
	//  new project
	void OnNew(wxCommandEvent& event);
	//  open project
	void OnOpen(wxCommandEvent& event);
	//  save project
	void OnSave(wxCommandEvent& event);
	//  quit application
	void OnQuit(wxCommandEvent& event);
	// toolbar event handlers
	//  select tool clicked
	void OnSelectToolClicked(wxCommandEvent& event);
	//  edit tool clicked
	void OnEditToolClicked(wxCommandEvent& event);
	//  wire tool clicked
	void OnWireToolClicked(wxCommandEvent& event);
	//  pan tool clicked
	void OnPanToolClicked(wxCommandEvent& event);

private:
	// SimpleProject (MODEL)
	SimpleProject project;

	// window sizer
	wxBoxSizer* sizer{ nullptr };

	// SimpleTool object
	SimpleTool tool;

	// toolbar
	wxToolBar* toolbar{ nullptr };

	// User Panel
	wxPanel* userPanel{ nullptr };

	// SimpleCanvas instance (DELEGATE)
	SimpleCanvas* canvas{ nullptr };

	// project name indicator text controls
	wxStaticText* stFileDir{ nullptr };
	wxStaticText* stFileName{ nullptr };

	// event table declaration
	wxDECLARE_EVENT_TABLE();
	// DO NOT DECLARE ANYTHING ELSE HERE
};

#define MENU_H
#endif