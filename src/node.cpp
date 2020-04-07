// node.cpp
// --------
// defines the basic node class (see node.h) that
// currently only holds positional data and a
// string label (unused)

#include "node.h"

// constructors
//  default
Node::Node() {}
//  positional
Node::Node(float xPosition, float yPosition)
	: mPosX(xPosition), mPosY(yPosition) {}

// positional data getters/setters
float Node::GetX() const { 
	return mPosX; 
}
void Node::SetX(float xPosition) {
	mPosX = xPosition;
}
float Node::GetY() const {
	return mPosY;
}
void Node::SetY(float yPosition) {
	mPosY = yPosition;
}

// label getters/setters
const std::string& Node::GetLabel() const {
	return mLabel;
}
void Node::SetLabel(std::string& label) {
	mLabel = label;
}
