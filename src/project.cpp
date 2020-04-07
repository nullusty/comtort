#include "project.h"

#include <fstream>
#include <map>
#include <filesystem>

// constructor
Project::Project() {}

// set project name
void Project::SetName(std::string projectName)
{
	name = projectName;
}
// get project name
const std::string& Project::GetName() const
{
	return name;
}

// set project directory
void Project::SetDirectory(std::string& projectDirectory)
{
	dir = projectDirectory;
}
// get project directory
const std::string& Project::GetDirectory() const
{
	return dir;
}

// get path of project file
std::string Project::GetPath() const
{
	// path of containing directory
	std::filesystem::path parentPath(dir);
	// path of file
	std::filesystem::path filePath(name + ".tort");
	std::filesystem::path fullPath = parentPath / filePath;

	return fullPath.string();
}

// set path of project file
void Project::SetPath(std::string pathString) {
	std::filesystem::path projectPath(pathString);

	// name of project, without .tort extension
	SetName(projectPath.stem().string());
	// containing directory of project
	SetDirectory(projectPath.parent_path().string());
}

// create project, returns false on failure
bool Project::Create(std::shared_ptr<Network> pNetwork)
{
	// create the file
	std::fstream fileStream;
	//  open the fileStream
	fileStream.open(GetPath(), std::fstream::out | std::fstream::binary);
	//  on success, close and continue to save
	if (fileStream) {
		fileStream.close();
		return Save(pNetwork);
	}
	//  on failure, return false
	else
		return false;
}

// save project, returns false on failure
bool Project::Save(std::shared_ptr<Network> pNetwork)
{
	// open file for writing, overwrite contents
	std::fstream fileStream;
	fileStream.open(GetPath(), std::fstream::out 
		| std::fstream::binary | std::fstream::trunc);
	//  on failure, return false
	if (!fileStream) return false;

	// writing buffers
	//  buffer for writing nodal positional data to file
	float nodePosBuffer[2]{ 0.0f, 0.0f };
	//  buffer for writing wire positional data to file
	float wirePosBuffer[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
	//  buffer for writing serialized node* id's to file
	unsigned wireIdBuffer[2]{ 0, 0 };

	// object-id map for serialization
	std::map<std::shared_ptr<Node>, unsigned> oidMap;

	// network serialization
	//  write number of nodes to file
	unsigned numNodes = pNetwork->GetNumNodes();
	fileStream.write((const char*)&numNodes, sizeof(unsigned));
	//  add entry for (nullptr, 0)
	oidMap.insert(std::pair<std::shared_ptr<Node>, unsigned>(nullptr, 0));
	//  iterate through node pointers, offset inserted id by 1
	auto pNodes = pNetwork->GetNodes();
	for (unsigned id = 0; id < pNodes.size(); id++) {
		// populate oidMap: associate node* with id
		oidMap.insert(std::pair<std::shared_ptr<Node>, unsigned>
			(pNodes.at(id), id+1));
		// populate writing buffer
		nodePosBuffer[0] = pNodes.at(id)->GetX();
		nodePosBuffer[1] = pNodes.at(id)->GetY();
		// write buffer to file
		fileStream.write((const char*)nodePosBuffer, sizeof(float) * 2);
	}
	//  write number of wires to file
	unsigned numWires = pNetwork->GetNumWires();
	fileStream.write((const char*)&numWires, sizeof(unsigned));
	//  write wires to file using oidMap
	for (auto& pWire : pNetwork->GetWires()) {
		// get wire's positional data
		float posFromX = pWire->GetFromX();
		float posFromY = pWire->GetFromY();
		float posToX = pWire->GetToX();
		float posToY = pWire->GetToY();
		//  populate writing buffer
		wirePosBuffer[0] = posFromX;
		wirePosBuffer[1] = posFromY;
		wirePosBuffer[2] = posToX;
		wirePosBuffer[3] = posToY;

		// write positional data
		fileStream.write((const char*)wirePosBuffer, sizeof(float) * 4);

		// get wire's associated node id's from oidMap
		unsigned idFrom = oidMap.at(pWire->GetFrom());
		unsigned idTo = oidMap.at(pWire->GetTo());
		//  populate writing buffer
		wireIdBuffer[0] = idFrom; 
		wireIdBuffer[1] = idTo;

		// write node id's
		fileStream.write((const char*)wireIdBuffer, sizeof(unsigned) * 2);
	}

	// close fileStream
	fileStream.close();
	return true;
}
// load project, returns false on failure
bool Project::Load(std::shared_ptr<Network> pNetwork)
{
	// clear network (delete any existing nodes and wires)
	pNetwork->Clear();

	// open file for reading
	std::fstream fileStream;
	fileStream.open(GetPath(), std::fstream::in);
	// on failure, return false
	if (!fileStream) return false;

	// reading buffers
	//  buffer for reading nodal positional data to file
	float nodePosBuffer[2]{ 0.0f, 0.0f };
	//  buffer for reading wire positional data to file
	float wirePosBuffer[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
	//  buffer for reading serialized node* id's to file
	unsigned wireIdBuffer[2]{ 0, 0 };

	// network de-serialization
	//  read number of nodes
	unsigned numNodes{ 0 };
	fileStream.read((char*)&numNodes, sizeof(unsigned));
	//  populate Network's nodes
	for (unsigned id = 0; id < numNodes; id++) {
		// read node positional data
		fileStream.read((char *)nodePosBuffer, sizeof(float) * 2);

		// create node using that data
		pNetwork->AddNode(nodePosBuffer[0], nodePosBuffer[1]);
	}
	//  read number of wires
	unsigned numWires{ 0 };
	fileStream.read((char*)&numWires, sizeof(unsigned));
	//  populate Network's wires
		unsigned idFrom{ 0 }, idTo{ 0 };
		std::shared_ptr<Node> pNodeFrom{ nullptr }, pNodeTo{ nullptr };
	for (unsigned id = 0; id < numWires; id++) {
		// read wire positional data
		fileStream.read((char*)wirePosBuffer, sizeof(float) * 4);
		// read wire's node id data
		fileStream.read((char*)wireIdBuffer, sizeof(unsigned) * 2);
		//  set temporary ids
		idFrom = wireIdBuffer[0]; idTo = wireIdBuffer[1];

		// create wire using that data
		pNetwork->AddWireByPositions(wirePosBuffer[0], wirePosBuffer[1],
			wirePosBuffer[2], wirePosBuffer[3]);
		// get that wire's pointer (most recently added)
		auto pWire = pNetwork->GetWire(pNetwork->GetNumWires() - 1);
		//  set temporary node pointers, offset by 1 if not 0 (see Project::Save)
		if (idFrom == 0) pNodeFrom = nullptr;
		else pNodeFrom = pNetwork->GetNode(idFrom - 1);
		if (idTo == 0) pNodeTo = nullptr;
		else pNodeTo = pNetwork->GetNode(idTo - 1);
		// set that wire's node pointers
		pWire->SetFrom(pNodeFrom); pWire->SetTo(pNodeTo);
	}

	// close file
	fileStream.close();
	return true;
}