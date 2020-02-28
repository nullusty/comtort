// node.cpp
// --------
// defines the basic node class (see node.h) that
// currently only holds positional data 

#include "node.h"

Node::Node(wxPoint position)
	: pos(position)
{}

void Node::SetPosition(wxPoint position)
{
	pos = position;
}

wxPoint Node::GetPosition(void)
{
	return pos;
}
