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
	nodes.push_back(Node((double)position.x, (double)position.y));
}

// get const ref to nodes
const std::vector<Node>& SimpleProject::GetNodes(void)
{
	return nodes;
}

// delete node by index
void SimpleProject::DeleteNodeByIndex(std::size_t index) {
	nodes.erase(nodes.begin() + index);
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

	// write nodes to file
	double buffer[2]{ 0.0,0.0 };
	for (auto&& node : nodes) {
		// read into buffer
		buffer[0] = node.x;
		buffer[1] = node.y;
		// write buffer
		outFile.Write(buffer, sizeof(buffer));
	}

	// close file
	outFile.Close();

	return true;
}
// load project, returns false on failure
bool SimpleProject::Load(void)
{
	// open file
	wxFile outFile(GetPath(), wxFile::read);
	if (!outFile.IsOpened()) {
		return false;
	}

	// get file size
	wxFileOffset nSize = outFile.Length();
	if (nSize == wxInvalidOffset) {
		return false;
	}

	// read nodes from file
	double buffer[2]{ 0.0,0.0 };
	while(!outFile.Eof()) {
		// read buffer
		outFile.Read(buffer, sizeof(buffer));
		// read into buffer
		nodes.push_back(Node(buffer[0], buffer[1]));
	}

	// close file
	outFile.Close();

	return true;
}