// node.h
// ------
// declares the basic node class

#ifndef NODE_H

struct Node {
	// global coordinates
	float x;
	float y;

	Node(float xPos, float yPos);
};

#define NODE_H
#endif