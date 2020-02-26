// menu.cpp
// --------
// defines SimpleMenu (see menu.h), the main window
// of comtort, as well as SimpleMenu's static event table 
// via wxBEGIN_EVENT_TABLE/wxEND_EVENT_TABLE

#include "menu.h"
#include "wx/wfstream.h"

// SimpleMenu constructor
SimpleMenu::SimpleMenu(const wxString& title)
	: wxFrame(nullptr, -1, title, wxPoint(-1,-1), wxSize(640, 480))
{
	// menubar creation
	auto menubar = new wxMenuBar;

	// menu: file
	auto file = new wxMenu;
	//	new project
	file->Append(wxID_NEW, wxT("&New"));
	//	open project
	file->Append(wxID_OPEN, wxT("&Open"));
	//	save project
	file->Append(wxID_SAVE, wxT("&Save"));
	file->AppendSeparator();
	//  exit application
	file->Append(wxID_EXIT, wxT("&Quit"));
	// attach to menubar
	menubar->Append(file, wxT("&File"));

	// set menubar
	SetMenuBar(menubar);

	// project info panel
	auto vbox = new wxBoxSizer(wxVERTICAL);
	auto infoPanel = new wxPanel(this, -1);
	stFileName = new wxStaticText(infoPanel, -1, wxT("project name:"));
	stFileDir = new wxStaticText(infoPanel, -1, wxT("project dir:"));
	//  add text controls to vbox
	vbox->Add(stFileName, 0, 0, 0);
	vbox->Add(stFileDir, 0, 0, 0);
	vbox->SetSizeHints(infoPanel);
	//  set infopanel's sizer
	infoPanel->SetSizer(vbox);

	// Center frame in display
	Centre();
}

// event handlers
//  new project
void SimpleMenu::OnNew(wxCommandEvent& WXUNUSED(event)) 
{
	// save current filename (in case user changes idea)
	auto tempFileName = currFileName;

	// get new project name
	wxTextEntryDialog nameDialog(this, "project name", "", "", wxOK);
	if (nameDialog.ShowModal() == wxID_OK)
	{
		currFileName = nameDialog.GetValue() + wxT(".tort");
	}
	// user changed idea
	else {
		return;
	}

	// get containing directory
	wxDirDialog dirDialog(this, "project location", "");
	if (dirDialog.ShowModal() == wxID_OK)
	{
		currFileDir = dirDialog.GetPath();
	}
	// user changed idea
	else {
		// reset original filename
		currFileName = tempFileName;
		return;
	}

	// set full project path
	projectPath = currFileDir + wxT("\\") + currFileName;

	// update static text
	stFileName->SetLabel(wxString("project name: ") + currFileName);
	stFileDir->SetLabel(wxString("project location: ") + currFileDir);

	// create project file
	projectFile.Create(projectPath);
}
//  open project
void SimpleMenu::OnOpen(wxCommandEvent& WXUNUSED(event)) 
{
	wxFileDialog fileDialog(this, wxT("Open TORT file"), "", "",
		"TORT files (*.tort)|*.tort", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	// user changed idea
	if (fileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}

	// load project file chosen by user
	wxFileInputStream inputStream(fileDialog.GetPath());
	if (!inputStream.IsOk()) {
		wxLogError("Cannot open file '%s'.", fileDialog.GetPath());
		return;
	}

	// set current project strings
	projectPath = fileDialog.GetPath();
	currFileName = projectPath.AfterLast(wxS('\\'));
	currFileDir = projectPath.BeforeLast(wxS('\\'));

	// update static text
	stFileName->SetLabel(wxString("project name: ") + currFileName);
	stFileDir->SetLabel(wxString("project location: ") + currFileDir);

	// TODO: implement load project
}
//  save project
void SimpleMenu::OnSave(wxCommandEvent& WXUNUSED(event)) 
{
	// save project file
	wxFileInputStream outputStream(projectPath);
	if (!outputStream.IsOk()) {
		wxLogError("Cannot save project in file '%s'.", projectPath);
		return;
	}

	// TODO: implement save project
}
//  quit application
void SimpleMenu::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

// static event table definition
wxBEGIN_EVENT_TABLE(SimpleMenu, wxFrame)
	EVT_MENU(wxID_NEW, SimpleMenu::OnNew)
	EVT_MENU(wxID_OPEN, SimpleMenu::OnOpen)
	EVT_MENU(wxID_SAVE, SimpleMenu::OnSave)
	EVT_MENU(wxID_EXIT, SimpleMenu::OnQuit)
wxEND_EVENT_TABLE()
