#ifndef IForm
#define IForm

/*
	This class is used for creating objects that can take input from console 
	Object is inherited from TextLine
*/

#include "TextLine.h"
#include <iostream>
#include "isNextAfterIndentation.h"
#include "isOnIndentationException.h"
#include <algorithm>

class InputForm : public TextLine {
protected:
	bool gettingInput; // checks mode of object (can take input or not)
	int cursorPositionIndex; // checks position of cursor on object where inserting or
	// removing of  symbol will happen

	Color activeColor; // color when object is taking input
	Color deactiveColor; // color when object is not taking input
	Color cursorColor; // color of position where cursor is on object when it takes input
public:
	// constructors call TextLine class constructors and additionally set default colors,
	// put in not taking input mode
	InputForm();
	InputForm(const std::string &s);
	/////////////////////////////////////

	void takeInput(const INPUT_RECORD &event); // perform functions of taking input when 
	// button pressed during getting input

	bool isGettingInput(); // functions returns current mode
	void setActiveColor(Color c); // sets color when taking input
	void setDeactiveColor(Color c);	// sets color when not taking input
	void setCursorColor(Color c); // sets color of cursor on object when taking input
	void appearOnConsoleScreen(HANDLE &hndl) override; // draws object on Screen

	bool turnInputStateOn(const INPUT_RECORD &event); // sets object in taking input mode 
	//and returns true if mode change. Also puts cursor in position where mouse was clicked

	bool turnInputStateOn(); // sets object in taking input mode and returns true if mode changed.
	// cursor is put at the end of string

	bool turnInputStateOff(); // sets object in not taking input mode and returns true if mode changed
	void setCursorPositionIndex(const INPUT_RECORD &event); // puts cursor where mouse clicked
	virtual void setCursorPositionIndex(int index); // puts cursor in specified position
	
	std::string getStringWithSlashT(); // converts string that contains spaces after \t in string 
	// without them

	bool isOnIndentation(int index); // checks if index is on spaces after \t
	//bool isNextAfterIndentation(int index); // checks if index is after spaces of \t
	void removeIndentation(int slashTIndex); // removes \t when backspace is pressed for it
	int getCursorIndexPosition(); // returns cursor position on object
	int getVisibleStringSize(); // returns stringsize
	size_t findIndexInTextLine(int i); // find index of symbol in origin string that matches it
	// on string on screen
	//int findVisibleIndex(int i); //makes reverse of findIndexInTextLine(int i)
};


#endif //IForm
