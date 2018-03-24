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
	void changeState();
	void takeInput(const INPUT_RECORD &event, HANDLE &hndl);
	bool isGettingInput();
	void setActiveColor(Color c);
	void setDeactiveColor(Color c);
	void appearOnConsole(HANDLE &hndl) override;
};


#endif //IForm
