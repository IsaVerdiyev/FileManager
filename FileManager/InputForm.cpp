#include "InputForm.h"



InputForm::InputForm() : TextLine(), 
	activeColor{defaultActiveColor},
	deactiveColor {defaultDeactiveColor}
{
	setColor(deactiveColor);
}

InputForm::InputForm(const std::string &s) : TextLine(s),
	activeColor{ defaultActiveColor },
	deactiveColor{ defaultDeactiveColor }
{
	setColor(deactiveColor);
}

bool InputForm::isGettingInput() {
	return gettingInput;
}

void InputForm::takeInput(const INPUT_RECORD &event) {
	lengthChanged = true;
	if(event.Event.KeyEvent.wVirtualKeyCode == VK_BACK) {
		if (sentenceSymbols.size() != 1) {
			sentenceSymbols.erase(sentenceSymbols.end() - 2);
		}
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
		turnInputStateOff();
	}
	else {
		CHAR_INFO c;
		c.Char.AsciiChar = event.Event.KeyEvent.uChar.AsciiChar;
		c.Attributes = activeColor;
		getText().insert(getText().end() - 1, c);
	}
}

void InputForm::setActiveColor(Color c) {
	activeColor = c;
}
void InputForm::setDeactiveColor(Color c) {
	deactiveColor = c;
}

bool InputForm::turnInputStateOn() {
	if (!gettingInput) {
		gettingInput = true;
		setColor(activeColor);
		return true;
	}
	else {
		return false;
	}
}
bool InputForm::turnInputStateOff() {
	if (gettingInput) {
		gettingInput = false;
		setColor(deactiveColor);
		return true;
	}
	else {
		return false;
	}
}

void InputForm::appearOnConsoleScreen(HANDLE &hndl) {
	if (isGettingInput()) {
		setColor(activeColor);
	}
	else {
		setColor(deactiveColor);
	}
	TextLine::appearOnConsoleScreen(hndl);
}


