#ifndef IForm
#define IForm

#include "TextLine.h"
#include <iostream>

class InputForm : public TextLine {
	bool gettingInput;
	int cursorPositionIndex;
	Color activeColor;
	Color deactiveColor;
	Color cursorColor;
public:
	InputForm();
	InputForm(const std::string &s);
	void takeInput(const INPUT_RECORD &event);
	bool isGettingInput();
	void setActiveColor(Color c);
	void setDeactiveColor(Color c);
	void setCursorColor(Color c);
	void appearOnConsoleScreen(HANDLE &hndl) override;
	bool turnInputStateOn(const INPUT_RECORD &event);
	bool turnInputStateOn();
	bool turnInputStateOff();
	void setCursorPositionIndex(const INPUT_RECORD &event);
	void setCursorPositionIndex(int index);
	void setVisibleStringSize();
	void setStringAfterFinishingInput();
	std::string getStringWithSlashT();
	//bool checkIfSpacesOfTab(int index);
	bool isOnIndentation(int index);
	void removeIndentation(int slashTIndex);
	int getCursorIndexPosition();
	int getVisibleStringSize();
};


#endif //IForm
