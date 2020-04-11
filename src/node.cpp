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
{
	mPos.x = xPosition;
	mPos.y = yPosition;
}
//  positional (vec2f)
Node::Node(vec2f position)
	: mPos(position) {}

// positional data getters/setters
float Node::GetX() const { 
	return mPos.x; 
}
void Node::SetX(float xPosition) {
	mPos.x = xPosition;
}
float Node::GetY() const {
	return mPos.y;
}
void Node::SetY(float yPosition) {
	mPos.y = yPosition;
}
// vec2f
vec2f Node::GetPosition() const {
	return mPos;
}
void Node::SetPosition(vec2f position) {
	mPos = position;
}

// label getters/setters
const std::string& Node::GetLabel() const {
	return mLabel;
}
void Node::SetLabel(std::string& label) {
	mLabel = label;
}
