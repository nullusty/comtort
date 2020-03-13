// node.h
// ------
// declares the basic node class

#ifndef NODE_H

#include <vector>

class Node {
	// forward connections
	std::vector<Node*> nodes;

public:
	// global coordinates
	float x{ 0.0f };
	float y{ 0.0f };

	// constructor
	Node(float xPos, float yPos);

	// get const ref to forward list
	const std::vector<Node*>& GetNodes(void) const;

	// add node to forward list
	void AddNode(Node* node);
};

#define NODE_H
#endif