// comtort.cpp
// -----------
// defines ComtortApp (see comtort.h) and calls 
// wxIMPLEMENT_APP to set up the platform-dependent
// entry points (main or WinMain) implementing ComtortApp
 
// wxWidgets header files
//  For compilers that don't support precompilation,
//  include "wx/wx.h"
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// comtort header files
#include "comtort.h"	// for ComtortApp
#include "menu.h"		// for SimpleMenu
#include "project.h"	// for SimpleProject

// create platform-dependent main() or WinMain()
// entry points that implement MyApp
wxIMPLEMENT_APP(ComtortApp);

// wxApp overrides
//  OnInit: creates the application's main window
bool ComtortApp::OnInit() {
	// create a SimpleMenu window
	auto frame = new SimpleMenu("comtort");

	// set the top window (automatically deleted)
	SetTopWindow(frame);

	// show it
	frame->Show(true);

	// start event loop
	return true;
}
//  OnExit: clean up anything initialized in OnInit
//          that wasn't set with SetTopWinodw
int ComtortApp::OnExit()
{
	/* clean up here */

	return 0; // return value is ignored
}