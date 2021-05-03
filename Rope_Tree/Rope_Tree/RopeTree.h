/******************************************************************
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

File Name: RopeTree.h
Project: Lab 7 - Rope Tree
Author: Kevin Wright
Creation date: 5/2/2021
-----------------------------------------------------------------*/

#pragma once
#include <queue>
#include <string>

class RopeTree
{
public:
	RopeTree(int maxStringLength, std::string str);
	~RopeTree();
	friend std::ostream& operator << (std::ostream& out, RopeTree& tree);
	char operator [](size_t index);
	RopeTree& operator += (RopeTree&& rhs);

	void DebugPrint() const;
private:

	class RopeNode
	{
	public:
		enum class RopeNodeType
		{
			Interier,
			Leaf
		};
		RopeNode(size_t weight) : weight(weight) {}
		virtual ~RopeNode() {}

		size_t GetWeight() { return weight; }
		virtual char operator [](size_t index) = 0;

		virtual RopeNodeType GetNodeType() = 0;
	protected:
		size_t weight;
	};
	
	friend std::ostream& operator << (std::ostream& out, RopeTree::RopeNode* node);

	class RopeNodeInterior : public RopeNode
	{
	public:
		RopeNodeInterior(size_t weight, RopeNode* left, RopeNode* right = nullptr);
		RopeNodeInterior(size_t maxStringLength, size_t weight, RopeNode* left, RopeNode* right);
		~RopeNodeInterior();
		int Height() const;
		virtual RopeNode::RopeNodeType GetNodeType() { return RopeNode::RopeNodeType::Interier; }
		virtual char operator [](size_t index);

		RopeNode* lChild;
		RopeNode* rChild;
	private:
		int recursiveHeightCalculation(int height);
	};
	
	friend std::ostream& operator << (std::ostream& out, RopeNodeInterior* node);

	class RopeNodeLeaf : public RopeNode
	{
	public:
		friend std::ostream& operator << (std::ostream& out, RopeNodeLeaf* node);

		RopeNodeLeaf(std::string str);
		~RopeNodeLeaf();
		RopeNodeInterior* Split(size_t maxStringLength);
		virtual RopeNode::RopeNodeType GetNodeType() { return RopeNode::RopeNodeType::Leaf; }
		virtual char operator [](size_t index);
	private:
		std::string text;
	};
	
	friend std::ostream& operator << (std::ostream& out, RopeNodeLeaf* node);
	void CreatePrintQueueByDepth(RopeNode* subtreeRoot, std::queue<RopeNode*>& printQueue, int depth) const;
	std::queue<RopeNode*> CreatePrintQueue() const;
	void DebugPrintNodes(RopeNode* subtreeRoot) const;

	RopeNodeInterior* root;
	int maxStringLength;
};