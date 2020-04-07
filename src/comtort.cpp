#include "comtort.h"

// comtort headers
#include "book.h"			// for Book

// ComtortApp override definitions
//  application initialization
bool ComtortApp::OnInit() {
	// create a window
	auto frame = new Book("comtort");

	// set the top window (automatically deleted)
	SetTopWindow(frame);

	// show it
	frame->Show(true);

	// start event loop
	return true;
}
//  application destruction
int ComtortApp::OnExit() {
	// cleanup anything initialized in ComtortApp::OnInit
	// that wasn't set with wxApp::SetTopWindow

	return 0;	// return value ignored
}

// makes the application class ComtortApp 
// known to wxWidgets for dynamic construction
wxIMPLEMENT_APP(ComtortApp);