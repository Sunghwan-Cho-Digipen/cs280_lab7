/******************************************************************
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

File Name: Main.cpp
Project: Lab 7 - Rope Tree
Author: Kevin Wright
Creation date: 5/2/2021
-----------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <string>
#include "RopeTree.h"


void Test1() {
	RopeTree rope(12, "Hello my name is Kevin");
	std::cout << rope << std::endl;
	std::cout << "Index 0 = " << rope[0] << std::endl;
	std::cout << "Index 4 = " << rope[4] << std::endl;
	std::cout << "Index 10 = " << rope[10] << std::endl;
	std::cout << "Index 11 = " << rope[11] << std::endl;
	std::cout << "Index 18 = " << rope[18] << std::endl;
	std::cout << "Index 20 = " << rope[20] << std::endl << std::endl;
	rope.DebugPrint();
}

void Test2() {
	RopeTree rope(4, "Hello my name is Kevin");
	std::cout << rope << std::endl;
	std::cout << "Index 0 = " << rope[0] << std::endl;
	std::cout << "Index 4 = " << rope[4] << std::endl;
	std::cout << "Index 10 = " << rope[10] << std::endl;
	std::cout << "Index 11 = " << rope[11] << std::endl;
	std::cout << "Index 18 = " << rope[18] << std::endl;
	std::cout << "Index 20 = " << rope[20] << std::endl << std::endl;
	rope.DebugPrint();
}

void Test3() {
	RopeTree rope(4, "Hello my name is Kevin");
	std::cout << rope << std::endl;
	try {
		std::cout << "This operation should be O(n) = 1" << std::endl;
		std::cout << "Index 22 = ";
		std::cout << rope[22] << std::endl;
	}
	catch (std::runtime_error &e) {
		std::cout << e.what();
	}
}

void Test4() {
	RopeTree tree1(3, "This is tree 1;");
	tree1.DebugPrint();
	tree1 += RopeTree(3, "This is tree 2");
	std::cout << std::endl << tree1 << std::endl << std::endl;
	tree1.DebugPrint();
}

void Test5() {
	std::string array[] = { "CS280 ", "is ", "the ", "best ", "class ", "EVER!" };
	RopeTree tree1(5,"");
	for(int i = 0; i < sizeof(array) / sizeof(*array); i++) {
	    tree1 += RopeTree(5, array[i]);
	}
	std::cout << tree1 << std::endl;
	tree1.DebugPrint();
}

void Test6() {
	std::ifstream infile("Data.txt");
	RopeTree currTree(8, "");

	std::string buff;
	while (infile.eof() == false) {
		std::getline(infile, buff);
		buff.append("\n");
		currTree += RopeTree(8, buff);
	}
	std::cout << currTree;
}

int main(int argc, char** argv) {
	int test_num = -1;
#ifndef _MSC_VER
	std::cin >> test_num;
#endif

	void (*tests[])() = { Test1, Test2, Test3, Test4, Test5, Test6 };
	int numTests = static_cast<int>(sizeof(tests) / sizeof(*tests));

	if (test_num > 0 && test_num <= numTests) {
		tests[test_num - 1]();
	} else {
		for (int i = 0; i < numTests; i++) {
			tests[i]();
			std::cout << std::endl << std::endl;
		}
	}
}