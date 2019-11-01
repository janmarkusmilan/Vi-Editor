#include "ArrayStack.h"
#include "Command.h"
#include "Editor.h"
#include "LinkedList.h"
#include "LinkedStack.h"
#include "ListInterface.h"
#include "Node.h"
#include "PrecondViolatedExcept.h"
#include "StackInterface.h"

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>  // use _getch()

using namespace std;

int main()
{
	Editor myEditor;
	ifstream inFile, keywordsFile;

	inFile.open("SampleText.txt");  // open file
	keywordsFile.open("keywords.txt");  // open file

	if (!inFile && !keywordsFile)  // if file cannot be opened
	{
		cout << "File cannot be opened." << endl;
		system("Pause");
		return 1;
	}

	myEditor.readFile(inFile, keywordsFile);

	myEditor.printLines();
	char ch = _getche();
	// _getch() will not echo the input char (and doesn't buffer a return char)

	do {
		myEditor.processCommand(ch);
		system("cls");
		myEditor.printLines();
		ch = _getch();
	} while (ch != 'q');

	system("Pause");
	return 0;
}