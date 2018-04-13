#include "InputForm.h"



InputForm::InputForm() : TextLine(), 
	activeColor{defaultActiveColor},
	deactiveColor {defaultDeactiveColor},
	inputIndexColor {defaultInputIndexColor},
	gettingInput{ false }
{
	setColor(deactiveColor);
}

InputForm::InputForm(const std::string &s) : TextLine(s),
	activeColor{ defaultActiveColor },
	deactiveColor{ defaultDeactiveColor },
	gettingInput{ false }
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
			setVisibleStringSize();
			resizeAccordingToMinLength();
		}
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
		turnInputStateOff();
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_LEFT) {
		if (inputIndexInLine != 0) {
			setInputIndex(inputIndexInLine - 1);
		}
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) {
		if (inputIndexInLine != sentenceSymbols.size() - 1) {
			setInputIndex(inputIndexInLine + 1);
		}
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
		setVisibleStringSize();
		setStringAfterFinishingInput();
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

void InputForm::setVisibleStringSize() {
	int counter = sentenceSymbols.size() - 1;
	while (sentenceSymbols[counter].Char.AsciiChar == ' ' && counter != 0) {
		counter--;
	}
	stringSize = counter + 1;
}

void InputForm::setStringAfterFinishingInput() {
	textInLine = getVisibleString();
	for (int i = 0; i < slashT_positions.size(); i++) {
		textInLine.erase(textInLine.begin() + slashT_positions[i], textInLine.begin() + slashT_positions[i] + TextLine::slashT_SpaceCounts);
	}
	/*for (int i = 0; i < slashT_positions.size(); i++) {
		sentenceSymbolsWithSlashT.erase(sentenceSymbolsWithSlashT.begin()  + slashT_positions[i], sentenceSymbolsWithSlashT.begin() + slashT_positions[i] + TextLine::slashT_SpaceCounts);
		CHAR_INFO c;
		c.Char.AsciiChar = '\t';
		sentenceSymbolsWithSlashT.insert(sentenceSymbolsWithSlashT.begin() + slashT_positions[i], c);
	}
	for (int i = 0; i < sentenceSymbolsWithSlashT.size(); i++) {
		textInLine.push_back(sentenceSymbolsWithSlashT[i].Char.AsciiChar);
	}*/
}

std::string InputForm::getVisibleString() {
	std::string visibleString;
	for (int i = 0; i < stringSize; i++) {
		visibleString.push_back(sentenceSymbols[i].Char.AsciiChar);
	}
	return visibleString;
}


