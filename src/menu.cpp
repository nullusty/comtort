// menu.cpp
// --------
// defines SimpleMenu (see menu.h), the main window
// of comtort, as well as SimpleMenu's static event table 
// via wxBEGIN_EVENT_TABLE/wxEND_EVENT_TABLE

#include "menu.h"

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

	// user view panel
	auto userPanel = new wxPanel(this, -1);
	//  sizer (vertical columns)
	auto vbox = new wxBoxSizer(wxVERTICAL);
	//  static text indicators of current project
	stFileName = new wxStaticText(userPanel, -1, wxT("project name:"));
	stFileDir = new wxStaticText(userPanel, -1, wxT("project dir:"));
	//  add text controls to vbox
	vbox->Add(stFileName, 0, 0, 0);
	vbox->Add(stFileDir, 0, 0, 0);
	//  create canvas
	canvas = new SimpleCanvas((wxFrame*)userPanel);
	//  bind event from canvas?
	//  add canvas to vbox
	vbox->Add(canvas, 0, 0, 0);
	//  set wsizer hints
	vbox->SetSizeHints(userPanel);
	//  set sizer
	userPanel->SetSizer(vbox);

	// Center frame in display
	Centre();
}

// event handlers
//  new project
void SimpleMenu::OnNew(wxCommandEvent& WXUNUSED(event)) 
{
	// save current filename (in case user changes idea)
	auto tempFileName = project.GetName();

	// get new project name
	auto nameDialog = new wxTextEntryDialog(this, "project name", "", "", wxOK);
	if (nameDialog->ShowModal() == wxID_OK)
	{
		project.SetName(nameDialog->GetValue());
	}
	// user changed idea
	else {
		nameDialog->Destroy();
		return;
	}
	nameDialog->Destroy();

	// get containing directory
	auto dirDialog = new wxDirDialog(this, "project location", "");
	if (dirDialog->ShowModal() == wxID_OK)
	{
		project.SetDirectory(dirDialog->GetPath());
	}
	// user changed idea
	else {
		// reset original filename
		project.SetName(tempFileName);
		dirDialog->Destroy();
		return;
	}
	dirDialog->Destroy();

	// save empty project
	if (!project.Create()) {
		wxLogError("Cannot save file '%s'.", project.GetPath());
		return;
	}

	// update static text
	stFileName->SetLabel(wxString("project name: ") + project.GetName());
	stFileDir->SetLabel(wxString("project location: ") + project.GetDirectory());
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

	// set project name and containing directory
	auto projectPath = fileDialog.GetPath();
	project.SetName(projectPath.AfterLast(wxS('\\')).BeforeFirst(wxS('.')));
	project.SetDirectory(projectPath.BeforeLast(wxS('\\')));

	// load project
	if (!project.Load()) {
		wxLogError("Cannot open file '%s'.", fileDialog.GetPath());
		return;
	}

	// update static text
	stFileName->SetLabel(wxString("project name: ") + project.GetName());
	stFileDir->SetLabel(wxString("project location: ") + project.GetDirectory());
}
//  save project
void SimpleMenu::OnSave(wxCommandEvent& WXUNUSED(event)) 
{
	// save project file
	if (!project.Save()) {
		wxLogError("Cannot save file '%s'.", project.GetPath());
		return;
	}
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
