// node.h
// ------
// declares the basic node class

#ifndef NODE_H

struct Node {
	// exists globally on [0,1]^2
	double xPos;
	double yPos;

	Node(double x, double y);
};

#define NODE_H
#endif