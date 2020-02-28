#ifndef PROJECT_H

#include <vector>
#include "node.h"

// project data structure
class SimpleProject {
private:
	// project name (excluding .tort)
	wxString name{ "" };
	// project directory (containing directory in filesystem)
	wxString dir{ "" };

	// example project data
	//  list of nodes
	std::vector<Node> nodes{};

public:
	// constructor
	SimpleProject();

	// set project name
	void SetName(wxString& projectName);
	// get project name
	const wxString& GetName(void);
	// set project directory
	void SetDirectory(wxString& projectDirectory);
	// get project directory
	const wxString& GetDirectory(void);

	// get path of project file
	wxString GetPath(void);

	// add node by position
	void AddNodeByPosition(wxPoint position);
	// get nodes
	const std::vector<Node>& GetNodes(void);

	// save project
	bool Create(void);
	// save project
	bool Save(void);
	// load project
	bool Load(void);
};

#define PROJECT_H
#endif