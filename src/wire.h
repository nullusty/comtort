// wire.h
// ------
// Declares the wire class, which (optionally) connects
// two nodes. Contains an internal endpoint structure.

#ifndef WIRE_H

#include "node.h"

#include <memory>

class Wire {
	// internal Endpoint structure
	class Endpoint {
		// shared_ptr to associated Node
		std::shared_ptr<Node> mpNode{ nullptr };

		// position of Endpoint
		float mPosX{ 0.0f };
		float mPosY{ 0.0f };

	public:
		// constructors
		//  default
		Endpoint();
		//  positional
		Endpoint(float xPosition, float yPosition);
		//  nodal
		Endpoint(std::shared_ptr<Node> pNode);
		//  nodal and positional
		Endpoint(std::shared_ptr<Node> pNode,
			float xPosition, float yPosition);

		// setters/getters for Node*
		void SetNodePtr(std::shared_ptr<Node> pNode);
		std::shared_ptr<Node> GetNodePtr() const;

		// setters/getters for position
		void SetX(float xPosition);
		float GetX() const;
		void SetY(float yPosition);
		float GetY() const;

		// auxillary setters
		//  associate with a node and copy its position
		void Associate(std::shared_ptr<Node>& pNodeToModify, std::shared_ptr<Node> pNodeToCopy);
		//  dissociate from the current node and copy its position
		void Dissociate();
	};

	// associated Endpoints
	Endpoint mTo;
	Endpoint mFrom;

public:
	// constructors
	//  default
	Wire();
	//  bi-nodal
	Wire(std::shared_ptr<Node> pNodeFrom,
		std::shared_ptr<Node> pNodeTo);
	//  rooted
	Wire(std::shared_ptr<Node> pNodeFrom, float toPositionX,
		float toPositionY);
	//  positional
	Wire(float fromPositionX, float fromPositionY,
		float toPositionX, float toPositionY);
	//  bi-nodal and positional
	Wire(std::shared_ptr<Node> pNodeFrom,
		float fromPositionX, float fromPositionY,
		std::shared_ptr<Node> pNodeTo,
		float toPositionX, float toPositionY);

	// Endpoint interface
	//  from endpoint's node* setters/getters
	void SetFrom(std::shared_ptr<Node> pNodeFrom);
	std::shared_ptr<Node> GetFrom() const;
	//  from endpoint's position setters/getters
	float GetFromX() const;
	void SetFromX(float xPosition);
	float GetFromY() const;
	void SetFromY(float yPosition);
	//  to endpoint's node* setters/getters
	void SetTo(std::shared_ptr<Node> pNodeTo);
	std::shared_ptr<Node> GetTo() const;
	//  to endpoint's position setters/getters
	float GetToX() const;
	void SetToX(float xPosition);
	float GetToY() const;
	void SetToY(float yPosition);
	//  auxillary setters
	void UnsetFrom();
	void UnsetTo();
	void Unset();
};

#define WIRE_H
#endif
