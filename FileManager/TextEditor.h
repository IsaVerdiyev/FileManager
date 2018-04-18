#ifndef TxtEdt
#define TxtEdt

/*
	Class is inherited from TemplateMenu class with TextEditorInputForm as object instead of T.
	This class performs functionality of editing text of file
*/

#include "TemplateMenu.h"
#include "TextEditorInputForm.h"


class TextEditor : public TemplateMenu<TextEditorInputForm> {
	int inputLineIndex; // specifies line which is taking input
	Color activeColor; // color when line is taking input
	Color deactiveColor; // color when line is not taking input
	Color cursorColor; // color of cursor when line is taking input
	std::string pathToFile; // string that containts absolute path to file
	int lengthOfLineInTextEditor = 0; // minimum length of line in text editor
public:
	TextEditor();
	TextEditor(const std::vector<std::string> &itemsVector);

	//setMenuItems override same function in TemplateMenu.
	//difference is here that the length of items is set to lengthOfLineInTextEditor.
	//in TemplateMenu class this function sets length of items to the longest string in array
	void setMenuItems(const std::vector<std::string> &itemsVector);

	
	void setInputLineIndex(int index);// sets index of line which takes input
	int getInputLineIndex(); // gets index of line which takes input

	// these functions just set colors in which future objects will be displayed on Screen
	void setActiveColor(Color c); 
	void setDeactiveColor(Color c);
	void setCursorColor(Color c);


	void setColors(); // sets colors of objects in array
	void addNewLine(HANDLE hndl); // adds new line on screen (program calls this function 
	//when user press Enter

	void removeLine(HANDLE hndl); // removes line from screen (program calls this function
	//when user presses backspace in the beginning of line)

	void setPathToFile(const std::string &path); // sets string of pathToFile variable
	void saveFile(); // saves text in file


	void setMenuItemButtons(); // sets objects of class according to strings in menuItemStrings.
	//This  function is overriden :
	// //difference is here that the length of items is set to lengthOfLineInTextEditor.
	//in TemplateMenu class this function sets length of items to the longest string in array
	
	
	void setLengthOfLineInTextEditor(int l); // sets value in variable lengthOfLineInTextEditor

	int getLengthOfLineInTextEditor();  // returns value in varialbe lengthOfLineInTextEditor
};




#endif
