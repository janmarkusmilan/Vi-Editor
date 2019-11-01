#ifndef EDITOR_
#define EDITOR_

#include "BinarySearchTree.h"
#include "Command.h"
#include "LinkedList.h"
#include "LinkedStack.h"
#include "ListInterface.h"
#include "Node.h"
#include "StackInterface.h"
#include <string>

using namespace std;

class Editor
{
public:
	Editor();
	void readFile(ifstream&, ifstream&);
	void saveFile();
	void printLines();
	void processCommand(char);
	void goToXY(int, int);
	void colorText(int);

private:
	LinkedList<string> myList;
	LinkedStack<Command> uStack;
	BinarySearchTree<string>* keywordsBST;
	Command commandObject;
	int numLines;
	int currentLine;
	int currentPos;
	int lineLength;
	string readLine, myLine, input;
};

#endif