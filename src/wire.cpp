// wire.cpp
// --------
// defines the Wire class as well as its internal
// Endpoint class. 

#include "wire.h"

// --- internal endpoint structure definitions ---
// constructors
//  default
Wire::Endpoint::Endpoint() {}
//  positional
Wire::Endpoint::Endpoint(float xPosition, float yPosition)
	: mPosX(xPosition), mPosY(yPosition) {}
//  nodal (associates)
Wire::Endpoint::Endpoint(std::shared_ptr<Node> pNode) {
	Associate(mpNode, pNode);
}
//  nodal and positional
Wire::Endpoint::Endpoint(std::shared_ptr<Node> pNode,
	float xPosition, float yPosition)
	: mPosX(xPosition), mPosY(yPosition)
{
	Associate(mpNode, pNode);
	mPosX = xPosition; mPosY = yPosition;
}

// node* getters/setters
void Wire::Endpoint::SetNodePtr(std::shared_ptr<Node> pNode) {
	Associate(mpNode, pNode);
}
std::shared_ptr<Node> Wire::Endpoint::GetNodePtr() const {
	return mpNode;
}

// setters/getters for positional data
void Wire::Endpoint::SetX(float xPosition) {
	mPosX = xPosition;
}
float Wire::Endpoint::GetX() const {
	// track with the associated node, 
	// should it exist
	if (mpNode) {
		return mpNode->GetX();
	}
	return mPosX;
}
void Wire::Endpoint::SetY(float yPosition) {
	mPosY = yPosition;
}
float Wire::Endpoint::GetY() const {
	// track with the associated node, 
	// should it exist
	if (mpNode) {
		return mpNode->GetY();
	}
	return mPosY;
}

// auxillary setters
//  associate with a node and copy its position
void Wire::Endpoint::Associate(std::shared_ptr<Node>& pNodeToModify, 
	std::shared_ptr<Node> pNodeToCopy) 
{
	// associate with the node
	pNodeToModify = pNodeToCopy;

	// if there is truly an associated node, copy its position
	if (pNodeToModify) {
		mPosX = pNodeToModify->GetX();
		mPosY = pNodeToModify->GetY();
	}
}
//  dissociate from the current node and copy its position
void Wire::Endpoint::Dissociate() {
	// copy the node's position, if possible
	if (mpNode) {
		mPosX = mpNode->GetX();
		mPosY = mpNode->GetY();
	}

	// clear the node*
	mpNode = nullptr;
}
// --- end of internal endpoint structure definitions ---

// constructors
//  default
Wire::Wire() {}
//  bi-nodal
Wire::Wire(std::shared_ptr<Node> pNodeFrom, std::shared_ptr<Node> pNodeTo)
	: mFrom(pNodeFrom), mTo(pNodeTo) {}
//  rooted
Wire::Wire(std::shared_ptr<Node> pNodeFrom, float toPositionX,
	float toPositionY)
	: mFrom(pNodeFrom), mTo(toPositionX, toPositionY) {}
//  positional
Wire::Wire(float fromPositionX, float fromPositionY, 
	float toPositionX, float toPositionY)
	: mFrom(fromPositionX, fromPositionY), mTo(toPositionX, toPositionY) {}
//  bi-nodal and positional
Wire::Wire(std::shared_ptr<Node> pNodeFrom,
	float fromPositionX, float fromPositionY,
	std::shared_ptr<Node> pNodeTo,
	float toPositionX, float toPositionY)
	: mFrom(pNodeFrom), mTo(pNodeTo)
{
	mFrom.SetX(fromPositionX); mFrom.SetY(fromPositionY);
	mTo.SetX(toPositionX); mTo.SetY(toPositionY);
}

// Endpoint interface
//  from endpoint's node* setters/getters
void Wire::SetFrom(std::shared_ptr<Node> pNodeFrom) {
	mFrom.SetNodePtr(pNodeFrom);
}
std::shared_ptr<Node> Wire::GetFrom() const {
	return mFrom.GetNodePtr();
}
//  from endpoint's position setters/getters
float Wire::GetFromX() const {
	return mFrom.GetX();
}
void Wire::SetFromX(float xPosition) {
	mFrom.SetX(xPosition);
}
float Wire::GetFromY() const {
	return mFrom.GetY();
}
void Wire::SetFromY(float yPosition) {
	mFrom.SetY(yPosition);
}
//  to endpoint's node* setters/getters
void Wire::SetTo(std::shared_ptr<Node> pNodeTo) {
	mTo.SetNodePtr(pNodeTo);
}
std::shared_ptr<Node> Wire::GetTo() const {
	return mTo.GetNodePtr();
}
//  to endpoint's position setters/getters
float Wire::GetToX() const {
	return mTo.GetX();
}
void Wire::SetToX(float xPosition) {
	mTo.SetX(xPosition);
}
float Wire::GetToY() const {
	return mTo.GetY();
}
void Wire::SetToY(float yPosition) {
	mTo.SetY(yPosition);
}
//  auxillary setters
void Wire::UnsetFrom() {
	mFrom.Dissociate();
}
void Wire::UnsetTo() {
	mTo.Dissociate();
}
void Wire::Unset() {
	UnsetFrom(); UnsetTo();
}
