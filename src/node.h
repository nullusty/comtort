// node.h
// ------
// declares the basic node class

#ifndef NODE_H

#include <string>

class Node {
	// coordinates in 2D space
	float mPosX{ 0.0f }, mPosY{ 0.0f };

	// label
	std::string mLabel{ "" };

public:
	// constructors
	//  default
	Node();
	//  positional
	Node(float xPosition, float yPosition);

	// positional data getters/setters
	float GetX() const;
	void SetX(float xPosition);
	float GetY() const;
	void SetY(float yPosition);

	// label getters/setters
	const std::string& GetLabel() const;
	void SetLabel(std::string& label);
};

#define NODE_H
#endif