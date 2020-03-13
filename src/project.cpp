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
	selected.push_back(false);
}
// delete node by index
void SimpleProject::DeleteNodeByIndex(std::size_t index) {
	nodes.erase(nodes.begin() + index);
	selected.erase(selected.begin() + index);
}

// get const ref to nodes
// -> use if you want to call const methods of the nodes
const std::vector<Node>& SimpleProject::GetNodes(void) const
{
	return nodes;
}
// get ref to nodes
// -> use if you want to call a non-const method of the nodes
std::vector<Node>& SimpleProject::GetNodes(void)
{
	// cast away const from other getter
	return const_cast<std::vector<Node>&>(const_cast<const SimpleProject*>(this)->GetNodes());
	// NOTE: this is okay because the member function calling it is non-const
	//  the object itself is non-const, and casting away the const
	//  is allowed.
}

void SimpleProject::SelectNodeByIndex(std::size_t index) {
	selected.at(index) = true;
}
void SimpleProject::UnselectNodeByIndex(std::size_t index) {
	selected.at(index) = false;
}
void SimpleProject::UnselectNodes(void) {
	for (auto& val : selected) {
		val = false;
	}
}

const std::vector<bool>& SimpleProject::GetSelectedNodes(void) const
{
	return selected;
}

std::vector<bool>& SimpleProject::GetSelectedNodes(void)
{
	return const_cast<std::vector<bool>&>(const_cast<const SimpleProject*>(this)->GetSelectedNodes());
}

// link nodes by index,
//  adds a reference to forward list of nodes.at(indexFrom)
//  to nodes.at(indexTo)
void SimpleProject::LinkNodesByIndex(std::size_t indexFrom, std::size_t indexTo)
{
	nodes.at(indexFrom).AddNode(&nodes.at(indexTo));
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