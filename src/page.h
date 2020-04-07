#ifndef PAGE_H

// wxWidget headers
#include "wx/window.h"		// for wxWindow
#include "wx/notebook.h"	// for wxNotebook

// graph headers
#include "network.h"		// for Network
#include "project.h"		// for Project
#include "camera.h"			// for Camera

// comtort headers
#include "canvas.h"			// for Canvas

// C++ headers
#include <memory>			// for std::shared_ptr

class Book;
// Page class
class Page : public wxWindow {
	friend Book;

	// wxWidget objects
	Canvas* mpCanvas{ nullptr };

	// graph objects
	//  networkp
	std::shared_ptr<Network> mpNetwork{ nullptr };
	//  project
	std::shared_ptr<Project> mpProject{ nullptr };
	//  camera
	std::shared_ptr<Camera> mpCamera{ nullptr };
public:
	Page(wxNotebook* parent);
};

#define PAGE_H
#endif