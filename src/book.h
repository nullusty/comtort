#ifndef BOOK_H

// wxWidget headers
#include "wx/frame.h"		// for wxFrame
#include "wx/panel.h"		// for wxPanel
#include "wx/notebook.h"	// for wxNotebook

// comtort headers
#include "page.h"

// C++ headers
#include <vector>			// for std::vector
#include <string>			// for std::string

// Book class
class Book : public wxFrame {
	// notebook
	wxNotebook* mpNotebook;
	// pages (handled by notebook)
	std::vector<Page*> mpPages{};

public:
	Book(std::string title);

	void OnNew(wxCommandEvent& WXUNUSED(evt));
	void OnOpen(wxCommandEvent& WXUNUSED(evt));
	void OnSave(wxCommandEvent& WXUNUSED(evt));

	void OnExit(wxCommandEvent& WXUNUSED(evt));
};

#define BOOK_H
#endif