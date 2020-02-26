// comtort.h
// ---------
// declares our application ComtortApp (public wxApp)
// overriding wxApp::OnInit and wxApp::OnExit

#ifndef COMTORT_H

class ComtortApp : public wxApp {
public:
	// override declarations
	//  application initialization
	virtual bool OnInit();
	//  application deletion
	virtual int OnExit();
};

#define COMTORT_H
#endif
