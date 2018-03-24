#ifndef IForm
#define IForm

#include "ChoosableButton.h"
#include <iostream>

class InputForm : public ChoosableButton {
	bool gettingInput;
public:
	Color activeColor;
	Color deactiveColor;
	InputForm();
	InputForm(const std::string &s, Color c = SameColor);
	void changeState();
	void takeInput(const INPUT_RECORD &event, HANDLE &hndl);
	bool isGettingInput();
	void setActiveColor(Color c);
	void setDeactiveColor(Color c);
	void eraseFromConsoleLastSymbol(HANDLE &hdnl, COORD &&pos);
};


#endif //IForm
