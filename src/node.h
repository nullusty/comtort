// node.h
// ------
// declares the basic node class

#ifndef NODE_H

#include "wx/gdicmn.h"

class Node {
private:
	wxPoint pos;

public:
	Node(wxPoint position);

	wxPoint GetPosition(void);
	void SetPosition(wxPoint position);
};

#define NODE_H
#endif