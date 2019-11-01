#include "Command.h"
#include "Editor.h"
#include "Windows.h"
#include <string>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <vector>
#include <sstream>
#include <iterator>

Editor::Editor()
{
	currentLine = 1;
	currentPos = 1;
	numLines = 0;
}  // end of default constructor

void Editor::readFile(ifstream& dataIn, ifstream& keywords)
{
	for (int i = 1; !dataIn.eof(); i++)  // read file line by line
	{
		getline(dataIn, readLine);
		lineLength = readLine.length();
		myList.insert(i, readLine);
		numLines++;
	}

	keywordsBST = new BinarySearchTree<string>();

	while (!keywords.eof())
	{
		keywords >> readLine;  // get line
		keywordsBST->add(readLine);
	}
}  // end of readFile

//This method prints the text in inputText
// It will print keywords in blue
// and have a cursor thing that follows you as you navigate
void Editor::printLines() 
{
	int i; // j

	for (i = 1; i < numLines + 1; i++) 
	{
		myLine = myList.getEntry(i);
		/*
		if (i == currentLine) {
			cout << "^" << inputText.getEntry(i) << endl;
			for (j = 1; j < currentPos; j++) {
				cout << " ";
			}
			cout << " ^" << endl;
		}
		else
			cout << " " << inputText.getEntry(i) << endl;
		*/
	   
		istringstream streamWords{ myLine };

		vector<string> words {
			istream_iterator<string>(streamWords), istream_iterator<string>()
		};

		bool isKeyword;
		for (string currentWord : words)
		{
			isKeyword = keywordsBST->contains(currentWord);
			
			if (isKeyword)
				colorText(FOREGROUND_BLUE| FOREGROUND_RED | FOREGROUND_INTENSITY |
					BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | 0X80);  //magenta
			else
				//colorText(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | 0X80);
				colorText(0XF0);

			cout << " " << currentWord;
		}
		cout << endl;
	}
	goToXY(currentPos, currentLine - 1);
}


void Editor::goToXY(int column, int line) 
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord);
}

void Editor::colorText(int value) 
{
	COORD coord;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	FlushConsoleInputBuffer(hConsole);
	SetConsoleTextAttribute(hConsole, value);
}

void Editor::processCommand(char inputCommand)
{
	if (inputCommand == 'h' || inputCommand == 75)  //left
	{
		if (currentPos > 1) 
		{
			currentPos--;
		}
	}

	else if (inputCommand == 'j' || inputCommand == 80)  // down
	{
		if (currentLine < numLines) 
			currentLine++;

		readLine = myList.getEntry(currentLine);  //read current line
		lineLength = readLine.length();  // get length

		if (currentPos > lineLength)
			currentPos = lineLength;
	}

	else if (inputCommand == 'k' || inputCommand == 72)  // up
	{
		if (currentLine > 1) 
			currentLine--;

		readLine = myList.getEntry(currentLine);  // read current line
		lineLength = readLine.length();  // get length

		if (currentPos > lineLength)
			currentPos = lineLength;
	}

	else if (inputCommand == 'l' || inputCommand == 77)  // right
	{
		readLine = myList.getEntry(currentLine);  // read current line
		lineLength = readLine.length();  // get length 

		if (currentPos < lineLength)
			currentPos++;  
	}

	else if (inputCommand == 'd')  // delete current line
	{
		if (_getch() == 'd') 
		{
			commandObject.setValue(myList.getEntry(currentLine));
			commandObject.setLineNum(currentLine);
			commandObject.setCommand("dd");

			uStack.push(commandObject);  // push command onto stack
			myList.remove(currentLine);  // delete current line
			numLines--;  // decrememt total number of lines
		}
	}

	else if (inputCommand == 'I')  // insert at beginning
	{
		goToXY(1, numLines + 1);  // display at end
		cout << "INSERT MODE (I): ";

		string insertModeInput;
		getline(cin, insertModeInput);  // end on enter key

		// create a new node
		Node<string>* newNode = new Node<string>(insertModeInput);  
		myList.insert(currentLine, insertModeInput);

		commandObject.setValue(myList.getEntry(currentLine));
		commandObject.setLineNum(currentLine);
		commandObject.setCommand("I");

		uStack.push(commandObject);  // push command onto stack
		numLines++;  // increment total number of lines
	}

	else if (inputCommand == 'i')  // insert in line
	{
		commandObject.setValue(myList.getEntry(currentLine));
		commandObject.setLineNum(currentLine);
		commandObject.setCommand("i");
		uStack.push(commandObject);  // push command onto stack

		input = myList.getEntry(currentLine);
		goToXY(1, numLines + 1);  // go to end of file

		string userInput;
		const int ESC = 27;
		char ch;

		cout << "INSERT MODE (i): "; 
		while ((ch = _getche()) != ESC)	 // end on escape key
		{
			userInput += ch;
		}
	
		// create a new node
		Node<string>* newNode = new Node<string>(userInput);
		input.insert(currentPos - 1, userInput);  // insert in line
		myList.replace(currentLine, input);
	}

	else if (inputCommand == 'x')  // delete single char
	{	
		input = myList.getEntry(currentLine);
		lineLength = input.length();
		if (currentPos <= lineLength)  // doesn't delete extra spaces
		{
			commandObject.setValue(myList.getEntry(currentLine));
			commandObject.setLineNum(currentLine);
			commandObject.setCommand("x");
			uStack.push(commandObject);  // push command onto stack

			input = myList.getEntry(currentLine);
			input.erase(currentPos - 1, 1);  // erase function in string
			myList.replace(currentLine, input);
		}
	}

	else if (inputCommand == 'u')  // undo
	{
		if (!uStack.isEmpty())
		{
			if (commandObject.getCommand() == "dd")  // condition statement
			{
				myList.insert(commandObject.getLineNum(), commandObject.getValue());
				numLines++;
				uStack.pop();
			}

			if (commandObject.getCommand() == "I")  // condition statement
			{
				myList.remove(commandObject.getLineNum());
				numLines--;
				uStack.pop();
			}

			if (commandObject.getCommand() == "i")  // condition statement
			{
				myList.replace(commandObject.getLineNum(), commandObject.getValue());
				uStack.pop();
			}

			if (commandObject.getCommand() == "x")  // condition statement
			{
				myList.replace(commandObject.getLineNum(), commandObject.getValue());
				uStack.pop();
			}

			if (!uStack.isEmpty())  // update the values on Command object
			{
				Command copyCommand;
				copyCommand = uStack.peek();
				commandObject.setValue(copyCommand.getValue());
				commandObject.setLineNum(copyCommand.getLineNum());
				commandObject.setCommand(copyCommand.getCommand());
			}
		}
	}

	else if (inputCommand == ':')  // save file
	{
		if (_getch() == 'w')
		{
			saveFile();
			goToXY(1, numLines + 1);
			cout << "File Saved!";
			system("Pause>null");
		}
	}
}  // end of processCommand

// save / update file by pressing ':' and then 'w'
void Editor::saveFile()
{
	ofstream dataOut;
	dataOut.open("SampleText.txt");  // open file

	for (int i = 1; i <= numLines; i++)
	{
		dataOut << myList.getEntry(i);  // update the file

		if (i < numLines)  // prevent having an extra line at the end
			dataOut << endl;
	}

	dataOut.close();
}  // end of saveFile