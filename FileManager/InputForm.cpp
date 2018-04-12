#include "InputForm.h"



InputForm::InputForm() : TextLine(), 
	activeColor{defaultActiveColor},
	deactiveColor {defaultDeactiveColor},
	inputIndexColor {defaultInputIndexColor}
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
		if(inputIndexInLine != 0) {
			sentenceSymbols.erase(sentenceSymbols.begin() + inputIndexInLine - 1);
			setInputIndex(inputIndexInLine - 1);
			resizeAccordingToMinLength();
		}
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
		turnInputStateOff();
	}
	else {
		if (inputIndexInLine != sentenceSymbols.size() - 1
			&& sentenceSymbols[sentenceSymbols.size() - 1].Char.AsciiChar == ' '
			&& sentenceSymbols.size() == minLength) 
		{
			sentenceSymbols.erase(sentenceSymbols.end() - 1);
		}
		CHAR_INFO c;
		c.Char.AsciiChar = event.Event.KeyEvent.uChar.AsciiChar;
		c.Attributes = activeColor;
		getSentenceSymbols().insert(getSentenceSymbols().begin() + inputIndexInLine, c);

		setInputIndex(inputIndexInLine + 1);
	}
}

void InputForm::setActiveColor(Color c) {
	activeColor = c;
}
void InputForm::setDeactiveColor(Color c) {
	deactiveColor = c;
}

void InputForm::setInputIndexColor(Color c) {
	inputIndexColor = c;
}

bool InputForm::turnInputStateOn(const INPUT_RECORD &event) {
	if (!gettingInput) {
		gettingInput = true;
		setInputIndex(event);
		return true;
	}
	else {
		setInputIndex(event);
		return false;
	}
}

bool InputForm::turnInputStateOn() {
	if (!gettingInput) {
		gettingInput = true;
		setInputIndex(stringSize - 1);
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
		setStringSizeAfterFinishingInput();
		return true;
	}
	else {
		return false;
	}
}

void InputForm::appearOnConsoleScreen(HANDLE &hndl) {
	if (isGettingInput()) {
		setColor(activeColor);
		sentenceSymbols[inputIndexInLine].Attributes = inputIndexColor;
	}
	else {
		//setColor(deactiveColor);
	}
	TextLine::appearOnConsoleScreen(hndl);
}

void InputForm::setInputIndex(const INPUT_RECORD &event) {
	for (int i = 0; i < sentenceSymbols.size(); i++) {
		if (startPosition.X + i == event.Event.MouseEvent.dwMousePosition.X) {
			setInputIndex(i);
		}
	}
}

void InputForm::setInputIndex(int index) {
	if (index < 0 || index >= sentenceSymbols.size()) {
		throw std::runtime_error("Out of rang in sentenceSymbols array");
	}
	inputIndexInLine = index;
}

void InputForm::setStringSizeAfterFinishingInput() {
	int counter = sentenceSymbols.size() - 1;
	while (sentenceSymbols[counter].Char.AsciiChar == ' ' && counter != 0) {
		counter--;
	}
	stringSize = counter + 1;
}


