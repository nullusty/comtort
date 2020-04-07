#include "book.h"

// wxWidgets headers
#include "wx/menu.h"		// for wxMenu and wxMenuBar
#include "wx/textdlg.h"		// for wxTextEntryDialog
#include "wx/filedlg.h"		// for wxFileDialog
#include "wx/dirdlg.h"		// for wxDirDialog
#include "wx/log.h"			// for wxLogError
#include "wx/event.h"		// for Bind
#include "wx/sizer.h"		// for wxBoxSizer

// graph headers
#include "network.h"
#include "project.h"

// comtort headers
#include "canvas.h"

Book::Book(std::string title)
	: wxFrame(nullptr, wxID_ANY, title)
{
	// menubar
	// menu bar
	wxMenuBar* pMenuBar = new wxMenuBar();
	//  file menu
	wxMenu* pFileMenu = new wxMenu();
	pFileMenu->Append(wxID_NEW, _T("&New"));
	pFileMenu->Append(wxID_OPEN, _T("&Open"));
	pFileMenu->Append(wxID_SAVE, _T("&Save"));
	pFileMenu->AppendSeparator();
	pFileMenu->Append(wxID_EXIT, _T("&Exit"));
	//  append file menu, set menubar
	pMenuBar->Append(pFileMenu, _T("&File"));
	SetMenuBar(pMenuBar);

	//  perform menu bindings
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Book::OnNew, this, wxID_NEW);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Book::OnOpen, this, wxID_OPEN);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Book::OnSave, this, wxID_SAVE);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Book::OnExit, this, wxID_EXIT);

	// top-level panel
	wxPanel* pPanel = new wxPanel(this, wxID_ANY);

	// notebook widget
	mpNotebook = new wxNotebook(pPanel, wxID_ANY);
	mpPages.push_back(new Page(mpNotebook));
	mpPages.push_back(new Page(mpNotebook));
	mpNotebook->AddPage(mpPages.at(0), L"Project Tab 1");
	mpNotebook->AddPage(mpPages.at(1), L"Project Tab 2");

	// sizer for top-level panel
	wxBoxSizer* pPanelSizer = new wxBoxSizer(wxHORIZONTAL);
	pPanelSizer->Add(mpNotebook, 1, wxEXPAND);
	pPanel->SetSizer(pPanelSizer);

	// set frame sizer
	wxBoxSizer* pSizer = new wxBoxSizer(wxHORIZONTAL);
	pSizer->Add(pPanel, 1, wxEXPAND);
	SetSizerAndFit(pSizer);

	// center frame in display
	Centre();
}

void Book::OnNew(wxCommandEvent& WXUNUSED(evt)) {
	// new page
	mpPages.push_back(new Page(mpNotebook));

	// select page
	Page* pPage = mpPages.back();
	// select project and network
	std::shared_ptr<Project> pProject = pPage->mpProject;
	std::shared_ptr<Network> pNetwork = pPage->mpNetwork;

	// get new project name
	wxTextEntryDialog* pTextDialog = new wxTextEntryDialog(this,
		"Project name?", "", "", wxOK);
	if (pTextDialog->ShowModal() == wxID_OK) {
		pProject->SetName(std::string(pTextDialog->GetValue().mb_str()));
	}
	// user changed idea
	else {
		pTextDialog->Destroy();
		// delete page
		delete pPage; mpPages.pop_back();
		return;
	}
	pTextDialog->Destroy();

	// get containing directory
	wxDirDialog* pDirDialog = new wxDirDialog(this,
		"Project location?", "");
	if (pDirDialog->ShowModal() == wxID_OK)
	{
		pProject->SetDirectory(std::string(pDirDialog->GetPath().mb_str()));
	}
	// user changed idea
	else {
		pDirDialog->Destroy();
		// delete page
		delete pPage; mpPages.pop_back();
		return;
	}
	pDirDialog->Destroy();

	// save project
	if (!pProject->Create(pNetwork)) {
		wxLogError("Cannot save file '%s'.", pProject->GetPath());
		// delete page
		delete pPage; mpPages.pop_back();
	}

	// insert the page
	mpNotebook->AddPage(pPage, pProject->GetName());
	// select the page
	mpNotebook->SetSelection(mpPages.size() - 1);
	// paint the nodes
	pPage->mpCanvas->PaintNow();
}
void Book::OnOpen(wxCommandEvent& WXUNUSED(evt)) {
	// new page
	mpPages.push_back(new Page(mpNotebook));

	// select page
	Page* pPage = mpPages.back();
	// select project and network
	std::shared_ptr<Project> pProject = pPage->mpProject;
	std::shared_ptr<Network> pNetwork = pPage->mpNetwork;

	// get project path
	wxFileDialog* pFileDialog = new wxFileDialog(this, wxT("Open TORT file"), "", "",
		"TORT files (*.tort)|*.tort", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (pFileDialog->ShowModal() == wxID_OK) {
		pProject->SetPath(std::string(pFileDialog->GetPath().mb_str()));
	}
	// user changed idea
	else {
		pFileDialog->Destroy();
		// delete page
		delete pPage; mpPages.pop_back();
		return;
	}
	pFileDialog->Destroy();

	// load project
	if (!pProject->Load(pNetwork)) {
		wxLogError("Cannot open file '%s'.", pProject->GetPath());
		// delete page
		delete pPage; mpPages.pop_back();
		return;
	}

	// insert the page
	mpNotebook->AddPage(pPage, pProject->GetName());
	// select the page
	mpNotebook->SetSelection(mpPages.size() - 1);
	// paint the nodes
	pPage->mpCanvas->PaintNow();
}
void Book::OnSave(wxCommandEvent& WXUNUSED(evt)) {
	// select page
	Page* pPage = mpPages.at(mpNotebook->GetSelection());
	// select project and network
	std::shared_ptr<Project> pProject = pPage->mpProject;
	std::shared_ptr<Network> pNetwork = pPage->mpNetwork;

	// save project file
	if (!pProject->Save(pNetwork)) {
		wxLogError("Cannot save file '%s'.", pProject->GetPath());
	}
}

void Book::OnExit(wxCommandEvent& WXUNUSED(evt)) {
	Close(true);
}