#include "InputForm.h"



InputForm::InputForm() : TextLine(), 
	activeColor{defaultActiveColor},
	deactiveColor {defaultDeactiveColor},
	cursorColor {defaultCursorColor},
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
		if(cursorPositionIndex != 0) {
			try {
				if (!isOnIndentation(cursorPositionIndex - 1)) {
					sentenceSymbols.erase(sentenceSymbols.begin() + cursorPositionIndex - 1);
					setCursorPositionIndex(cursorPositionIndex - 1);
				}
			}
			catch (int index) {
				removeIndentation(index);
				setCursorPositionIndex(cursorPositionIndex - 4);
			}
			
		}
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
		turnInputStateOff();
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_LEFT) {
		if (cursorPositionIndex != 0) {
			try {
				if (!isOnIndentation(cursorPositionIndex - 1)) {
					setCursorPositionIndex(cursorPositionIndex - 1);
				}
			}
			catch (int) {};
		}
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) {
		if (cursorPositionIndex != sentenceSymbols.size() - 1) {
			try {
				if (!isOnIndentation(cursorPositionIndex + 1)) {
					setCursorPositionIndex(cursorPositionIndex + 1);
				}
			}
			catch (int) {};
		}
	}
	else {
		if (cursorPositionIndex != sentenceSymbols.size() - 1
			&& sentenceSymbols[sentenceSymbols.size() - 1].Char.AsciiChar == ' '
 			&& sentenceSymbols.size() == minLength) 
		{
			sentenceSymbols.erase(sentenceSymbols.end() - 1);
		}
		CHAR_INFO c;
		c.Char.AsciiChar = event.Event.KeyEvent.uChar.AsciiChar;
		c.Attributes = activeColor;
		getSentenceSymbols().insert(getSentenceSymbols().begin() + cursorPositionIndex, c);

		setCursorPositionIndex(cursorPositionIndex + 1);
		//setVisibleStringSize();
	}
	setVisibleStringSize();
	resizeAccordingToMinLength();
	if (cursorPositionIndex >= sentenceSymbols.size()) {
		cursorPositionIndex = sentenceSymbols.size() - 1;
	}
	setStringAfterFinishingInput();
}

void InputForm::setActiveColor(Color c) {
	activeColor = c;
}
void InputForm::setDeactiveColor(Color c) {
	deactiveColor = c;
}

void InputForm::setCursorColor(Color c) {
	cursorColor = c;
}

bool InputForm::turnInputStateOn(const INPUT_RECORD &event) {
	if (!gettingInput) {
		gettingInput = true;
		setCursorPositionIndex(event);
		return true;
	}
	else {
		setCursorPositionIndex(event);
		return false;
	}
}

bool InputForm::turnInputStateOn() {
	if (!gettingInput) {
		gettingInput = true;
		setCursorPositionIndex(stringSize);
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
		//setVisibleStringSize();
		//setStringAfterFinishingInput();
		return true;
	}
	else {
		return false;
	}
}

void InputForm::appearOnConsoleScreen(HANDLE &hndl) {
	if (isGettingInput()) {
		setColor(activeColor);
		sentenceSymbols[cursorPositionIndex].Attributes = cursorColor;
	}
	else {
		//setColor(deactiveColor);
	}
	TextLine::appearOnConsoleScreen(hndl);
}

void InputForm::setCursorPositionIndex(const INPUT_RECORD &event) {
	for (int i = 0; i < sentenceSymbols.size(); i++) {
		if (startPosition.X + i == event.Event.MouseEvent.dwMousePosition.X) {
			try {
				if (!isOnIndentation(i)) {
					setCursorPositionIndex(i);
				}
			}
			catch (int) {}
		}
	}
}

void InputForm::setCursorPositionIndex(int index) {
	if (index < 0 || index >= sentenceSymbols.size()) {
		throw std::runtime_error("Out of rang in sentenceSymbols array");
	}
	cursorPositionIndex = index;
}

void InputForm::setVisibleStringSize() {
	int counter = sentenceSymbols.size() - 1;
	while (sentenceSymbols[counter].Char.AsciiChar == ' ' && counter != 0) {
		counter--;
	}
	stringSize = counter + 1;
}

void InputForm::setStringAfterFinishingInput() {
	textInLine = getStringWithSlashT();
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

std::string InputForm::getStringWithSlashT() {
	std::string stringWithSlashT = getVisibleString();
	for (int i = 0; i < slashT_positions.size(); i++) {
		textInLine.erase(textInLine.begin() + slashT_positions[i], textInLine.begin() + slashT_positions[i] + TextLine::slashT_SpaceCounts);
	}
	return stringWithSlashT;
}

//bool InputForm::checkIfSpacesOfTab(int index) {
//	for (int i = 0; i < TextLine::slashT_SpaceCounts; i++) {
//		if (sentenceSymbols[index + i].Char.AsciiChar != ' ') {
//			return false;
//		}
//	}
//	return true;
//}


bool InputForm::isOnIndentation(int index) {
	std::string stringWithoutSlashIndentation = HelperFunctions::getStringWithReplacedSlashT_ToSpaces(textInLine, TextLine::slashT_SpaceCounts);
	for (int i = 0; i < slashT_positions.size(); i++) {
		for (int j = 0; j < TextLine::slashT_SpaceCounts; j++) {
			if (slashT_positions[i] + j == index) {
				throw i;
			}
		}
	}
	return false;
}

void InputForm::removeIndentation(int slashTIndex) {
	sentenceSymbols.erase(sentenceSymbols.begin() + slashT_positions[slashTIndex], sentenceSymbols.begin() + slashT_positions[slashTIndex] + TextLine::slashT_SpaceCounts);
	slashT_positions.erase(slashT_positions.begin() + slashTIndex);
}


int InputForm::getCursorIndexPosition() {
	return cursorPositionIndex;
}

int InputForm::getVisibleStringSize() {
	return stringSize;
}


