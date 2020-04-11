// node.h
// ------
// declares the basic node class

#ifndef NODE_H

// graph headers
#include "math_defs.h"		// for vec2f

// C++ headers
#include <string>			// for std::string

class Node {
	// coordinates in 2D space
	vec2f mPos{ 0.0f, 0.0f };

	// label
	std::string mLabel{ "" };

public:
	// constructors
	//  default
	Node();
	//  positional
	Node(float xPosition, float yPosition);
	//  positional (vec2f)
	Node(vec2f position);

	// positional data getters/setters
	float GetX() const;
	void SetX(float xPosition);
	float GetY() const;
	void SetY(float yPosition);
	// vec2f
	vec2f GetPosition() const;
	void SetPosition(vec2f position);

	// label getters/setters
	const std::string& GetLabel() const;
	void SetLabel(std::string& label);
};

#define NODE_H
#endif