#ifndef COMTORT_H

#include "wx/app.h"		// for wxApp

class ComtortApp : public wxApp {
	// override declarations
	//  application initialization
	virtual bool OnInit();
	//  application destruction
	virtual int OnExit();
};

// creates a forward declaration of the
// of wxGetApp() function, implemented
// by wxIMPLEMENT_APP(ComtortApp) 
wxDECLARE_APP(ComtortApp);

#define COMTORT_H
#endif
