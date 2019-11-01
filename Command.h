//Write a Command class (with constructor, getters, setters)
//The class should contain a command and a possible value
//Here is a list of some commands:
//https://www.cs.colostate.edu/helpdocs/vi.html

#ifndef COMMAND_
#define COMMAND_

#include <string>

using namespace std;

class Command
{
public:
	Command();
	Command(string, string);
	string getCommand() const;
	void setCommand(string);
	string getValue() const;
	void setValue(string);
	int getLineNum() const;
	void setLineNum(int);

private:
	string mCommand;
	string mValue;
	int mLineNum;
}; // end Command

#endif