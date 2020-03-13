// node.cpp
// --------
// defines the basic node class (see node.h) that
// currently only holds positional data 

#include "node.h"

Node::Node(float xPos, float yPos)
	: x(xPos), y(yPos)
{}

// get const ref to forward connections
const std::vector<Node*>& Node::GetNodes(void) const
{
	return nodes;
}

// add node to forward connections
void Node::AddNode(Node* node)
{
	nodes.push_back(node);
}
