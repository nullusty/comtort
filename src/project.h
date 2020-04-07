// project.h
// ---------
// Declares main abstract data structure: a Project. This contains
// methods for saving/loading a given Network to a file, as well as the
// file metadata.

#ifndef PROJECT_H

#include <string>

#include "network.h"

class Project {
private:
	// project name (excluding .tort)
	std::string name{ "" };
	// project directory (containing directory in filesystem)
	std::string dir{ "" };

public:
	// constructor
	Project();

	// project metadata setters/getters
	//  set project name
	void SetName(std::string projectName);
	//  get project name
	const std::string& GetName() const;
	//  set project directory
	void SetDirectory(std::string& projectDirectory);
	//  get project directory
	const std::string& GetDirectory() const;
	//  get full path of project file
	std::string GetPath() const;
	//  set full path of project file
	void SetPath(std::string pathString);

	// serialization/deserialization methods
	//  create a new project
	bool Create(std::shared_ptr<Network> pNetwork);
	//  save the current project
	bool Save(std::shared_ptr<Network> pNetwork);
	//  load an existing project
	bool Load(std::shared_ptr<Network> pNetwork);
};

#define PROJECT_H
#endif