#include "project.h"

#include "wx/file.h"

// constructor
SimpleProject::SimpleProject() {}

// set project name
void SimpleProject::SetName(wxString& projectName)
{
	name = projectName;
}
// get project name
const wxString& SimpleProject::GetName(void)
{
	return name;
}

// set project directory
void SimpleProject::SetDirectory(wxString& projectDirectory)
{
	dir = projectDirectory;
}
// get project directory
const wxString& SimpleProject::GetDirectory(void)
{
	return dir;
}

// get path of project file
wxString SimpleProject::GetPath(void)
{
	return dir + wxT('\\') + name + wxT(".tort");
}

// add node by position
void SimpleProject::AddNodeByPosition(wxPoint position)
{
	nodes.push_back(Node(position));
}

// get const ref to nodes
const std::vector<Node>& SimpleProject::GetNodes(void)
{
	return nodes;
}

// create project, returns false on failure
bool SimpleProject::Create(void)
{
	wxFile outFile;
	// create file (do not overwrite) with "wx" permissions
	outFile.Create(GetPath());
	// attempt to write to the file
	return Save();
}

// save project, returns false on failure
bool SimpleProject::Save(void)
{
	// open file
	wxFile outFile(GetPath(), wxFile::write);
	if (!outFile.IsOpened()) {
		return false;
	}

	for (auto& node : nodes) {
		// write each of the nodes
	}
	return true;
}
// load project, returns false on failure
bool SimpleProject::Load(void)
{
	// needs implementation
	return true;
}