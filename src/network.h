// network.h
// ---------
// Declares the Network class, which is responsible
// for creating/maintaining/deleting Node and Wire
// objects.

#ifndef NETWORK_H

// C++ headers
#include <cstddef>	// for std::size_t
#include <vector>	// for std::vector
#include <memory>	// for std::shared_ptr
#include <list>		// for std::list

// graph headers
#include "node.h"		// for Node
#include "wire.h"		// for Wire
#include "math_defs.h"	// for vec2f

// wxWidget headers
#include "wx/dc.h"	// for wxDC

// Handles a Network (graph) of Nodes and Wires
// holds up to 30 recent Commands
class Network {
public:
	class Command {
	public:
		virtual ~Command() {}
		virtual void execute(Network* pNetwork) = 0;
		virtual void undo(Network* pNetwork) = 0;
	};

	// Node objects (vertices)
	std::vector<std::shared_ptr<Node>> mpNodes{};
	// Wire objects (edges)
	std::vector<std::shared_ptr<Wire>> mpWires{};
	// Command objects
	std::vector<std::unique_ptr<Command>> mpCommands{};
	//  current position
	std::size_t mCommandPos{ 0 };

public:
	// constructors
	//  default
	Network();

	// Node interface
	//  get nodes
	const std::vector<std::shared_ptr<Node>>& GetNodes() const;
	//  add node
	void AddNode();
	void AddNode(float xPosition, float yPosition);
	void AddNode(std::shared_ptr<Node> pNode);
	//  get node ptr by index
	std::shared_ptr<Node> GetNode(std::size_t index);
	//  remove node by index
	void RemoveNode(std::size_t index);
	//  remove node by pointer
	void EraseNode(std::shared_ptr<Node> pNode);
	//  clear nodes
	void ClearNodes();
	//  number of nodes
	std::size_t GetNumNodes();
	
	// Undo/redoable node interface
	//  push node
	void PushNode(float xPosition, float yPosition);
	void PopNode();
	//  remove node
	void RemoveNodeCmd(std::shared_ptr<Node> pNode);
	//  remove nodes
//////  NEEDS MOVE SEMANTICS!!!
	void RemoveNodesCmd(std::vector<std::shared_ptr<Node>> pNodes);
	// move node
	void MoveNodeCmd(std::shared_ptr<Node> pNode, vec2f newPosition);
	void MoveNodesCmd(std::vector<std::shared_ptr<Node>> pNodes, vec2f trans);

	// Wire interface
	//  get wires
	const std::vector<std::shared_ptr<Wire>>& GetWires() const;
	//  add wire
	void AddWire();
	void AddWireByIndices(std::size_t indexFrom,
		std::size_t indexTo);
	void AddRootedWire(std::size_t indexFrom,
		float toPositionX, float toPositionY);
	void AddWireByPositions(float fromPositionX, float fromPositionY, 
		float toPositionX, float toPositionY);
	void AddWireByIndicesAndPositions(
		std::size_t indexFrom, float fromPositionX, float fromPositionY,
		std::size_t indexTo, float toPositionX, float toPositionY);
	void AddWireByNodePtrs(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
	//  get wire by index
	std::shared_ptr<Wire> GetWire(std::size_t index);
	//  remove wire by index
	void RemoveWire(std::size_t index);
	//  clear wires
	void ClearWires();
	//  get num wires
	std::size_t GetNumWires();

	// undo/redo-able wire interface
	void PopWire();
	void AddWireCmd(std::shared_ptr<Node> from, std::shared_ptr<Node> to);

	// auxillary setters, clears vectors and arrays
	void Clear();

	// Command (undo/redoable actions) interface
	//  undo and redo
	void Undo();
	void Redo();
	//  push/pop nodes
	void AddNodeCmd(float xPosition, float yPosition);
};

#define NETWORK_H
#endif
