#ifndef PROJECT_H

#include <vector>

#include "wx/file.h"
#include "wx/string.h"
#include "wx/gdicmn.h"

#include "node.h"

// project data structure
class SimpleProject {
private:
	// project name (excluding .tort)
	wxString name{ "" };
	// project directory (containing directory in filesystem)
	wxString dir{ "" };

	// project data
	//  list of nodes
	std::vector<Node> nodes{};
	//  selected nodes
	std::vector<bool> selected{};

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
	// delete node at index
	void DeleteNodeByIndex(std::size_t index);

	// get const ref to nodes
	const std::vector<Node>& GetNodes(void) const;
	// get ref to nodes
	std::vector<Node>& GetNodes(void);

	// select node by index
	void SelectNodeByIndex(std::size_t index);
	// unselect node by index
	void UnselectNodeByIndex(std::size_t index);
	// unselect all
	void UnselectNodes(void);

	// get const ref to selected nodes list
	const std::vector<bool>& GetSelectedNodes(void) const;
	// get ref to selected nodes list
	std::vector<bool>& GetSelectedNodes(void);

	// link nodes by position, returns false if cannot link
	void LinkNodesByIndex(std::size_t indexFrom, std::size_t indexTo);

	// save project
	bool Create(void);
	// save project
	bool Save(void);
	// load project
	bool Load(void);
};

#define PROJECT_H
#endif