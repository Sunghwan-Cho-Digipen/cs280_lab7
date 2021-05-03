/******************************************************************
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

File Name: RopeTree.cpp
Project: Lab 7 - Rope Tree
Author: Sunghwan Cho
Creation date: 05/03/2021
******************************************************************/

#include "RopeTree.h"

#include <iostream>

std::ostream& operator << (std::ostream& out, RopeTree& tree)
{
	out << tree.root;
	return out;
}

std::ostream& operator << (std::ostream& out, RopeTree::RopeNode* node)
{
	if (node->GetNodeType() == RopeTree::RopeNode::RopeNodeType::Interier)
	{
		out << static_cast<RopeTree::RopeNodeInterior*>(node);
	}
	if (node->GetNodeType() == RopeTree::RopeNode::RopeNodeType::Leaf)
	{
		out << static_cast<RopeTree::RopeNodeLeaf*>(node);
	}
	return out;
}

std::ostream& operator << (std::ostream& out, RopeTree::RopeNodeInterior* node)
{
	if (node->lChild != nullptr)
	{
		out << node->lChild;
	}
	if (node->rChild != nullptr)
	{
		out << node->rChild;
	}
	return out;
}

std::ostream& operator << (std::ostream& out, RopeTree::RopeNodeLeaf* node)
{
	out << node->text;
	return out;
}


void RopeTree::CreatePrintQueueByDepth(RopeTree::RopeNode* subtreeRoot, std::queue<RopeTree::RopeNode*>& printQueue, int depth) const
{
	if (depth == 0)
	{
		printQueue.push(subtreeRoot);
	}
	else if (subtreeRoot == nullptr)
	{
		CreatePrintQueueByDepth(nullptr, printQueue, depth - 1);
		CreatePrintQueueByDepth(nullptr, printQueue, depth - 1);
	}
	else
	{
		if (subtreeRoot->GetNodeType() == RopeNode::RopeNodeType::Interier)
		{
			CreatePrintQueueByDepth(static_cast<RopeNodeInterior*>(subtreeRoot)->lChild, printQueue, depth - 1);
			CreatePrintQueueByDepth(static_cast<RopeNodeInterior*>(subtreeRoot)->rChild, printQueue, depth - 1);
		}
		else
		{
			CreatePrintQueueByDepth(nullptr, printQueue, depth - 1);
			CreatePrintQueueByDepth(nullptr, printQueue, depth - 1);
		}
	}
}

std::queue<RopeTree::RopeNode*> RopeTree::CreatePrintQueue() const
{
	if (root == nullptr)
	{
		return std::queue<RopeNode*>();
	}

	std::queue<RopeNode*> result;
	const int HEIGHT = static_cast<RopeNodeInterior*>(root)->Height();
	for (int depth = 0; depth <= HEIGHT; depth++)
	{
		CreatePrintQueueByDepth(root, result, depth);
	}

	return result;
}

RopeTree::RopeTree(int maxStringLength, std::string str)
	: maxStringLength(maxStringLength)
{
	RopeNodeLeaf* currentLeaf = new RopeNodeLeaf(std::move(str));
	if (currentLeaf->GetWeight() > maxStringLength)
	{
		const int CURRENT_LEAF_WEIGHT = currentLeaf->GetWeight();
		RopeNodeInterior* currentInterior = currentLeaf->Split(maxStringLength);
		root = new RopeNodeInterior(CURRENT_LEAF_WEIGHT, currentInterior, nullptr);
	}
	else
	{
		root = new RopeNodeInterior(currentLeaf->GetWeight(), currentLeaf, nullptr);
	}
}

RopeTree::~RopeTree()
{
	delete root;
}

char RopeTree::operator[](size_t index)
{
	if (root->GetWeight() <= index)
	{
		throw std::runtime_error("Out of Bounds");
	}

	return (*root->lChild)[index];
}

RopeTree& RopeTree::operator+=(RopeTree&& rhs)
{
	root->rChild = rhs.root->lChild;
	const int WEIGHT = static_cast<int>(root->GetWeight() + rhs.root->GetWeight());
	rhs.root->lChild = nullptr;
	RopeNodeInterior* currentRoot = root;
	root = nullptr;
	root = new RopeNodeInterior(WEIGHT, currentRoot, nullptr);
	delete rhs.root;
	rhs.root = nullptr;
	return *this;
}

RopeTree::RopeNodeInterior::RopeNodeInterior(size_t weight, RopeNode* left, RopeNode* right)
	: RopeNode(weight), lChild(left), rChild(right)
{

}

RopeTree::RopeNodeInterior::RopeNodeInterior(size_t maxStringLength, size_t weight, RopeNode* left, RopeNode* right)
	: RopeNode(weight), lChild(left), rChild(right)
{
	if (lChild->GetWeight() > maxStringLength)
	{
		RopeNodeInterior* replaceInterior = reinterpret_cast<RopeNodeLeaf*>(left)->Split(maxStringLength);
		delete lChild;
		lChild = replaceInterior;
	}

	if (rChild->GetWeight() > maxStringLength)
	{
		RopeNodeInterior* replaceInterior = reinterpret_cast<RopeNodeLeaf*>(right)->Split(maxStringLength);
		delete rChild;
		rChild = replaceInterior;
	}
}

RopeTree::RopeNodeInterior::~RopeNodeInterior()
{
	delete lChild;
	lChild = nullptr;
	delete rChild;
	rChild = nullptr;
}

int RopeTree::RopeNodeInterior::Height() const
{
	const int LEFT_VALUE = (lChild == nullptr) ? -1 : ((lChild->GetNodeType() == RopeNodeType::Leaf) ? 0 : reinterpret_cast<RopeNodeInterior*>(lChild)->Height());
	const int RIGHT_VALUE = (rChild == nullptr) ? -1 : ((rChild->GetNodeType() == RopeNodeType::Leaf) ? 0 : reinterpret_cast<RopeNodeInterior*>(rChild)->Height());

	return LEFT_VALUE > RIGHT_VALUE ? LEFT_VALUE + 1 : RIGHT_VALUE + 1;
}

char RopeTree::RopeNodeInterior::operator[](size_t index)
{
	int weight = this->GetWeight();
	if (weight > index)
	{
		return (*lChild)[index];
	}

	return (*rChild)[index - GetWeight()];
}

RopeTree::RopeNodeLeaf::RopeNodeLeaf(std::string str)
	: RopeNode(str.length()), text(str)
{
}

RopeTree::RopeNodeLeaf::~RopeNodeLeaf()
{
	text.clear();
}

RopeTree::RopeNodeInterior* RopeTree::RopeNodeLeaf::Split(size_t maxStringLength)
{
	// This means that this->GetWeight() must be bigger than the maxStringLength
	const int NEXT_FIRST_WEIGHT = static_cast<int>(weight / 2);
	const std::string FIRST_NODE_STRING = text.substr(0, NEXT_FIRST_WEIGHT);
	const std::string SECOND_NODE_STRING = text.substr(NEXT_FIRST_WEIGHT);
	RopeNodeInterior* returnInterior
		= new RopeNodeInterior(maxStringLength, NEXT_FIRST_WEIGHT,
			new RopeNodeLeaf(FIRST_NODE_STRING), new RopeNodeLeaf(SECOND_NODE_STRING));
	return returnInterior;
}

char RopeTree::RopeNodeLeaf::operator[](size_t index)
{
	return text[index];
}

void RopeTree::DebugPrint() const
{
	std::queue<RopeNodeLeaf*> outputStrings;
	if (root != nullptr)
	{
		std::queue<RopeNode*> printQueue = CreatePrintQueue();

		std::cout << "height: " << static_cast<RopeNodeInterior*>(root)->Height() << std::endl;

		int nodeRowCount = 0;
		int nodeRowTarget = 1;
		int coutWidth = 2 << static_cast<RopeNodeInterior*>(root)->Height();
		while (printQueue.empty() == false)
		{
			if (nodeRowCount == 0)
			{
				std::cout.width(coutWidth / 2); // first node of each row should have a 1/2 width offset
			}
			else
			{
				std::cout.width(coutWidth - 1);	// set the width for the next element
			}

			if (printQueue.front() != nullptr)
			{
				std::cout << printQueue.front()->GetWeight();
			}
			else
			{
				std::cout << "X";
			}

			// check if we are done with the current row
			if (++nodeRowCount == nodeRowTarget)
			{
				std::cout << std::endl;
				nodeRowTarget *= 2;
				coutWidth /= 2;
				nodeRowCount = 0;
			}
			else
			{
				std::cout << ' ';
			}
			printQueue.pop();
		}
	}
	else
	{
		std::cout << "Tree is Empty" << std::endl;
	}
	DebugPrintNodes(root);

	std::cout << std::endl;
}

void RopeTree::DebugPrintNodes(RopeNode* subtreeRoot) const
{
	if (subtreeRoot->GetNodeType() == RopeTree::RopeNode::RopeNodeType::Interier)
	{
		RopeTree::RopeNodeInterior* nodePtr = static_cast<RopeTree::RopeNodeInterior*>(subtreeRoot);
		if (nodePtr->lChild != nullptr)
		{
			DebugPrintNodes(nodePtr->lChild);
		}
		if (nodePtr->rChild != nullptr)
		{
			DebugPrintNodes(nodePtr->rChild);
		}
	}
	else
	{
		std::cout << '[' << static_cast<RopeTree::RopeNodeLeaf*>(subtreeRoot) << ']';
	}
}