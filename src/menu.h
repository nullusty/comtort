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
#include "wx/file.h"

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

private:
	// current project directory
	wxString currFileDir{ "" };
	// current project filename
	wxString currFileName{ "" };
	// full path
	wxString projectPath{ "" };

	// project name indicator text controls
	wxStaticText* stFileDir{ nullptr };
	wxStaticText* stFileName{ nullptr };

	// project file
	wxFile projectFile;

	// event table declaration
	wxDECLARE_EVENT_TABLE();
	// DO NOT DECLARE ANYTHING ELSE HERE
};

#define MENU_H
#endif