#ifndef IForm
#define IForm

#include "TextLine.h"
#include <iostream>

class InputForm : public TextLine {
	bool gettingInput;
public:
	Color activeColor;
	Color deactiveColor;
	InputForm();
	InputForm(const std::string &s);
	void takeInput(const INPUT_RECORD &event);
	bool isGettingInput();
	void setActiveColor(Color c);
	void setDeactiveColor(Color c);
	void appearOnConsoleScreen(HANDLE &hndl) override;
	bool turnInputStateOn();
	bool turnInputStateOff();
};


#endif //IForm
