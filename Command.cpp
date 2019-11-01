#include "Command.h"
#include <string>

using namespace std;

Command::Command() : mCommand(""), mValue("")  //Set to null
{
}  // end default constructor

Command::Command(string com, string val)
{
	mCommand = com;
	mValue = val;
}

string Command::getCommand() const
{
	return mCommand;
}  // end getCommand

void Command::setCommand(string inCommand)
{
	mCommand = inCommand;
}  // end setCommand

string Command::getValue() const
{
	return mValue;
}  // end getValue

void Command::setValue(string inValue)
{
	mValue = inValue;
}  // end setValue

int Command::getLineNum() const
{
	return mLineNum;
}

void Command::setLineNum(int inLineNum)
{
	mLineNum = inLineNum;
}