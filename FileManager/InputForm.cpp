#include "InputForm.h"



InputForm::InputForm() : ChoosableButton(), 
	activeColor{defaultActiveColor},
	deactiveColor {defaultDeactiveColor}
{}

InputForm::InputForm(const std::string &s, Color c) : ChoosableButton(s, c),
	activeColor{ defaultActiveColor },
	deactiveColor{ defaultDeactiveColor }
{}

bool InputForm::isGettingInput() {
	return gettingInput;
}

void InputForm::takeInput(const INPUT_RECORD &event, HANDLE &hndl) {
	CHAR_INFO c;
	if(event.Event.KeyEvent.wVirtualKeyCode == VK_BACK) {
		if (sentenceSymbols.size() != 1) {
			eraseFromConsoleLastSymbol(hndl, COORD{ static_cast<short>(startPosition.X + sentenceSymbols.size() - 1), startPosition.Y });
			sentenceSymbols.erase(sentenceSymbols.end() - 2);
		}
	}
	else {
		c.Char.AsciiChar = event.Event.KeyEvent.uChar.AsciiChar;
		getText()[getText().size() - 1] = c;
		c.Char.AsciiChar = '\0';
		getText().push_back(c);
		setColor(activeColor);
	}
}

void InputForm::setActiveColor(Color c) {
	activeColor = c;
}
void InputForm::setDeactiveColor(Color c) {
	deactiveColor = c;
}

void InputForm::changeState() {
	if (gettingInput) {
		gettingInput = false;
		setColor(deactiveColor);
	}
	else {
		gettingInput = true;
		setColor(activeColor);
	}
}

void InputForm::eraseFromConsoleLastSymbol(HANDLE &hndl, COORD &&pos) {
	SetConsoleTextAttribute(hndl, defaultBackground);
	SetConsoleCursorPosition(hndl, pos);
	std::cout << '\0';
	SetConsoleCursorPosition(hndl, { 0, 0 });
}
