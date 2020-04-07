// network.cpp
// -----------
// Defines the Network class.

#include "network.h"

#include "math_defs.h"

// constructors
//  default
Network::Network() {}

// Node interface
//  get nodes
const std::vector<std::shared_ptr<Node>>& Network::GetNodes() const {
	return mpNodes;
}
//  add node
void Network::AddNode() {
	mpNodes.push_back(std::make_shared<Node>());
}
void Network::AddNode(float xPosition, float yPosition) {
	mpNodes.push_back(std::make_shared<Node>(xPosition, yPosition));
}
void Network::AddNode(std::shared_ptr<Node> pNode) {
	mpNodes.push_back(pNode);
}
//  get node ptr by index
std::shared_ptr<Node> Network::GetNode(std::size_t index) {
	return mpNodes.at(index);
}
//  remove node by index
void Network::RemoveNode(std::size_t index) {
	mpNodes.erase(mpNodes.begin() + index);
}
//  clear nodes
void Network::ClearNodes() {
	mpNodes.clear();
}
//  number of nodes
	// internal command structure
std::size_t Network::GetNumNodes() {
	return mpNodes.size();
}
//  push nodes
void Network::PushNode(float xPosition, float yPosition) {
	mpNodes.push_back(std::make_shared<Node>(xPosition, yPosition));
}
void Network::PopNode() {
	mpNodes.pop_back();
}
void Network::EraseNode(std::shared_ptr<Node> pNode) {
	// delete pNode from list of nodes ptrs
	for (auto it = mpNodes.begin(); it != mpNodes.end(); ++it) {
		if (*it == pNode) {
			mpNodes.erase(it);
			break;
		}
	}
}

// Wire interface
// get wires
const std::vector<std::shared_ptr<Wire>>& Network::GetWires() const {
	return mpWires;
}
//  add wire
void Network::AddWire() {
	mpWires.push_back(std::make_shared<Wire>());
}
void Network::AddWireByIndices(std::size_t indexFrom,
	std::size_t indexTo)
{
	mpWires.push_back(std::make_shared<Wire>(mpNodes.at(indexFrom), mpNodes.at(indexTo)));
}
void Network::AddRootedWire(std::size_t indexFrom,
	float toPositionX, float toPositionY)
{
	mpWires.push_back(std::make_shared<Wire>(mpNodes.at(indexFrom), toPositionX, toPositionY));
}
void Network::AddWireByPositions(float fromPositionX, float fromPositionY,
	// internal command structure
	float toPositionX, float toPositionY)
{
	mpWires.push_back(std::make_shared<Wire>(fromPositionX, fromPositionY, 
		toPositionX, toPositionY));
}
void Network::AddWireByIndicesAndPositions(
	std::size_t indexFrom, float fromPositionX, float fromPositionY,
	std::size_t indexTo, float toPositionX, float toPositionY)
{
	mpWires.push_back(std::make_shared<Wire>(
		mpNodes.at(indexFrom), fromPositionX, fromPositionY,
		mpNodes.at(indexTo), toPositionX, toPositionY));
}
void Network::AddWireByNodePtrs(std::shared_ptr<Node> from, std::shared_ptr<Node> to) {
	mpWires.push_back(std::make_shared<Wire>(from, to));
}

//  get wire by index
std::shared_ptr<Wire> Network::GetWire(std::size_t index) {
	return mpWires.at(index);
}
//  remove wire by index
void Network::RemoveWire(std::size_t index) {
	mpWires.erase(mpWires.begin() + index);
}
//  clear wires
void Network::ClearWires() {
	mpWires.clear();
}
//  get num wires
std::size_t Network::GetNumWires() {
	return mpWires.size();
}

// undo/redo-able wire interface
void Network::PopWire() {
	mpWires.pop_back();
}
// auxillary setters
void Network::Clear() {
	ClearWires(); ClearNodes();
}

// Command interface
class AddNodeCommand : public Network::Command {
	float mPosX, mPosY;

public:
	AddNodeCommand(float xPosition, float yPosition)
		: mPosX(xPosition), mPosY(yPosition) {}

	virtual void execute(Network* pNetwork) {
		pNetwork->PushNode(mPosX, mPosY);
	}

	virtual void undo(Network* pNetwork) {
		pNetwork->PopNode();
	}
};

class RemoveNodeCommand : public Network::Command {
	// captured state of node
	std::shared_ptr<Node> mpNode;
	// wires that point to that node
	//  from
	std::vector<std::shared_ptr<Wire>> mpWiresFrom{};
	//  to
	std::vector<std::shared_ptr<Wire>> mpWiresTo{};

public:
	RemoveNodeCommand(std::shared_ptr<Node> pNode)
		: mpNode(pNode) {}

	virtual void execute(Network* pNetwork) {
		// set wire refs to nullptr
		for (auto& pWire : pNetwork->GetWires()) {
			if (pWire->GetFrom() == mpNode) {
				mpWiresFrom.push_back(pWire);
				pWire->UnsetFrom();
			}
			else if (pWire->GetTo() == mpNode) {
				mpWiresTo.push_back(pWire);
				pWire->UnsetTo();
			}
		}
		// remove the node
		pNetwork->EraseNode(mpNode);
	}

	virtual void undo(Network* pNetwork) {
		// add it back
		pNetwork->AddNode(mpNode);
		// reset wire refs
		for (auto& pWire : mpWiresFrom) {
			pWire->SetFrom(mpNode);
		}
		for (auto& pWire : mpWiresTo) {
			pWire->SetTo(mpNode);
		}
		// clear wire vectors
		mpWiresFrom.clear();
		mpWiresTo.clear();

	}
};

class RemoveNodesCommand : public Network::Command {
	std::vector<std::unique_ptr<RemoveNodeCommand>> mpCmds{};

public:
	RemoveNodesCommand(std::vector<std::shared_ptr<Node>> pNodes)
	{
		for (auto& pNode : pNodes) {
			mpCmds.push_back(std::make_unique<RemoveNodeCommand>(pNode));
		}
	}

	virtual void execute(Network* pNetwork) {
		for (auto& pCmd : mpCmds) {
			pCmd->execute(pNetwork);
		}
	}

	virtual void undo(Network* pNetwork) {
		for (auto& pCmd : mpCmds) {
			pCmd->undo(pNetwork);
		}
	}
};

class AddWireCommand : public Network::Command {
	// to and from
	std::shared_ptr<Node> mTo;
	std::shared_ptr<Node> mFrom;

public:
	AddWireCommand(std::shared_ptr<Node> to, std::shared_ptr<Node> from)
		: mTo(to), mFrom(from) {}

	virtual void execute(Network* pNetwork) {
		pNetwork->AddWireByNodePtrs(mFrom, mTo);
	}

	virtual void undo(Network* pNetwork) {
		pNetwork->PopWire();
	}
};

class MoveNodeCommand : public Network::Command {
	// which node
	std::shared_ptr<Node> mpNode;
	// original position
	vec2f mPosOld;
	// new position
	vec2f mPosNew;

public:
	MoveNodeCommand(std::shared_ptr<Node> pNode, vec2f newPosition)
		: mpNode(pNode), mPosOld({ pNode->GetX(), pNode->GetY() }),
		mPosNew(newPosition) {}

	virtual void execute(Network* pNetwork) {
		mpNode->SetX(mPosNew.x);
		mpNode->SetY(mPosNew.y);
	}

	virtual void undo(Network* pNetwork) {
		mpNode->SetX(mPosOld.x);
		mpNode->SetY(mPosOld.y);
	}
};

void Network::MoveNodeCmd(std::shared_ptr<Node> pNode, vec2f newPosition) {
	// user is overwriting the future, delete any future commands
	if (mCommandPos < mpCommands.size()) mpCommands.resize(mCommandPos);

	// add a new command to the list
	mpCommands.push_back(
		std::make_unique<MoveNodeCommand>(pNode, newPosition));
	// execute
	mpCommands.at(mCommandPos)->execute(this);
	// iterate forwards
	++mCommandPos;
}

void Network::AddNodeCmd(float xPosition, float yPosition) {
	// user is overwriting the future, delete any future commands
	if (mCommandPos < mpCommands.size()) mpCommands.resize(mCommandPos);

	// add a new command to the list
	mpCommands.push_back(
		std::make_unique<AddNodeCommand>(xPosition, yPosition));
	// execute
	mpCommands.at(mCommandPos)->execute(this);
	// iterate forwards
	++mCommandPos;
}

void Network::RemoveNodeCmd(std::shared_ptr<Node> pNode) {
	// user is overwriting the future, delete any future commands
	if (mCommandPos < mpCommands.size()) mpCommands.resize(mCommandPos);

	// add a new command to the list
	mpCommands.push_back(
		std::make_unique<RemoveNodeCommand>(pNode));
	// execute
	mpCommands.at(mCommandPos)->execute(this);
	// iterate forwards
	++mCommandPos;
}

void Network::RemoveNodesCmd(std::vector<std::shared_ptr<Node>> pNodes) {
	// user is overwriting the future, delete any future commands
	if (mCommandPos < mpCommands.size()) mpCommands.resize(mCommandPos);

	// add a new command to the list
	mpCommands.push_back(
		std::make_unique<RemoveNodesCommand>(pNodes));
	// execute
	mpCommands.at(mCommandPos)->execute(this);
	// iterate forwards
	++mCommandPos;
}

void Network::AddWireCmd(std::shared_ptr<Node> from, std::shared_ptr<Node> to) {
	// user is overwriting the future, delete any future commands
	if (mCommandPos < mpCommands.size()) mpCommands.resize(mCommandPos);

	// add a new command to the list
	mpCommands.push_back(
		std::make_unique<AddWireCommand>(from, to));
	// execute
	mpCommands.at(mCommandPos)->execute(this);
	// iterate forwards
	++mCommandPos;
	
}

void Network::Undo() {
	// don't go further back than is possible
	if (mCommandPos == 0) return;

	// iterate backwards to the most recent command
	--mCommandPos;
	// undo the current command
	mpCommands.at(mCommandPos)->undo(this);
}

void Network::Redo() {
	// don't go further forward than is possible
	if (mCommandPos == mpCommands.size()) return;

	// redo the current command
	mpCommands.at(mCommandPos)->execute(this);
	// iterate forwards
	++mCommandPos;
}
