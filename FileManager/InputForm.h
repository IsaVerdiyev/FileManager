#ifndef IForm
#define IForm

#include "TextLine.h"
#include <iostream>

class InputForm : public TextLine {
	bool gettingInput;
	int inputIndexInLine;
	Color activeColor;
	Color deactiveColor;
	Color inputIndexColor;
public:
	InputForm();
	InputForm(const std::string &s);
	void takeInput(const INPUT_RECORD &event);
	bool isGettingInput();
	void setActiveColor(Color c);
	void setDeactiveColor(Color c);
	void setInputIndexColor(Color c);
	void appearOnConsoleScreen(HANDLE &hndl) override;
	bool turnInputStateOn(const INPUT_RECORD &event);
	bool turnInputStateOn();
	bool turnInputStateOff();
	void setInputIndex(const INPUT_RECORD &event);
	void setInputIndex(int index);
	void setVisibleStringSize();
	void setStringAfterFinishingInput();
	std::string getStringWithSlashT();
	bool checkIfSpacesOfTab(int index);
	bool isOnIndentation(int index);
	void removeIndentation(int slashTIndex);
};


#endif //IForm
