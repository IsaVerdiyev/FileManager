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
	if (event.Event.KeyEvent.wVirtualKeyCode == VK_BACK) {
		if (getCursorIndexPosition() != 0) {
			if (getVisibleStringSize() >= getCursorIndexPosition()) {
				int indexInLine = findIndexInTextLine(cursorPositionIndex - 1);
				if (textInLine[indexInLine] == '\t') {
					cursorPositionIndex -= 4;
				}
				else {
					cursorPositionIndex--;
				}
				textInLine.erase(textInLine.begin() + indexInLine);
				setTextAndColor(textInLine);
			}
			else {
				cursorPositionIndex--;
			}
		}
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
		turnInputStateOff();
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_LEFT) {
		if (cursorPositionIndex != 0) {
			setCursorPositionIndex(cursorPositionIndex - 1);
		}
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) {
		if (getCursorIndexPosition() != getSentenceSymbols().size() - 1) {
			try {
				if (!isOnIndentation(getCursorIndexPosition() + 1)) {
					setCursorPositionIndex(getCursorIndexPosition() + 1);
				}
			}
			catch (isOnIndentationException exc) {
				int resultCursorPositionIndex = exc.getIndex() + TextLine::slashT_SpaceCounts;
				setCursorPositionIndex(resultCursorPositionIndex);
			}
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
		getSentenceSymbols().insert(getSentenceSymbols().begin() + getCursorIndexPosition(), c);
		if (c.Char.AsciiChar == '\t') {
			CHAR_INFO space;
			space.Char.AsciiChar = ' ';
			for (int i = 0; i < TextLine::slashT_SpaceCounts - 1; i++) {
				getSentenceSymbols().insert(getSentenceSymbols().begin() + getCursorIndexPosition() + 1, space);
			}
			cursorPositionIndex += 4;
		}
		else {
			cursorPositionIndex++;
		}
		std::string newString = getStringWithSlashT();
		setTextAndColor(newString);
	}
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
	}
	TextLine::appearOnConsoleScreen(hndl);
}

void InputForm::setCursorPositionIndex(const INPUT_RECORD &event) {
	for (int i = 0; i < sentenceSymbols.size(); i++) {
		if (startPosition.X + i == event.Event.MouseEvent.dwMousePosition.X) {
			setCursorPositionIndex(i);
		}
	}
}

void InputForm::setCursorPositionIndex(int index) {
	try {
		if (!isOnIndentation(index)) {
			if (index < 0 || index >= sentenceSymbols.size()) {
				throw std::runtime_error("Out of rang in sentenceSymbols array");
			}
			cursorPositionIndex = index;
		}
	}
	catch (isOnIndentationException exc) {
		setCursorPositionIndex(exc.getIndex());
	}
}



std::string InputForm::getStringWithSlashT() {
	std::string stringWithSlashT = getVisibleString();
	for (int i = 0; i < stringWithSlashT.size(); i++) {
		if (stringWithSlashT[i] == '\t' && i != stringWithSlashT.size() - 1) {
			stringWithSlashT.erase(stringWithSlashT.begin() + i + 1, stringWithSlashT.begin() + i + TextLine::slashT_SpaceCounts);
		}
	}
	return stringWithSlashT;
}




bool InputForm::isOnIndentation(int index) {
	for (int i = 1; i < TextLine::slashT_SpaceCounts; i++) {
		int j = index - i;
		if (j >= 0) {
			if (sentenceSymbols[j].Char.AsciiChar == '\t') {
				throw isOnIndentationException(j);
			}
		}
	}
	return false;
}

//bool InputForm::isNextAfterIndentation(int index) {
//	std::string stringWithoutSlashIndentation = HelperFunctions::getStringWithReplacedSlashT_ToSpaces(textInLine, TextLine::slashT_SpaceCounts);
//	int j = index - TextLine::slashT_SpaceCounts;
//	if (j > 0) {
//		if (sentenceSymbols[j].Char.AsciiChar == '\t') {
//			throw j;
//		}
//	}
//	return false;
//}

void InputForm::removeIndentation(int slashTIndex) {
	textInLine.erase(textInLine.begin() + slashTIndex);
	setTextAndColor(textInLine);
}

//int InputForm::findVisibleIndex(int index) {
//	int j = 0;
//	for (int i = 0; i < textInLine.size(); i++) {
//		if (i == index) {
//			break;
//		}
//		if(textInLine[i] == '\t') {
//			j += 4;
//		}
//		else {
//			j++;
//		}
//	}
//	return j;
//}
//

int InputForm::getCursorIndexPosition() {
	return cursorPositionIndex;
}

int InputForm::getVisibleStringSize() {
	return stringSize;
}

int InputForm::findIndexInTextLine(int visibleIndex) {
	std::string str = textInLine;
	int i = 0;
	for (int j = 0; i < str.size(); i++, j++) {
		if (visibleIndex == j) {
			break;
		}
		if (str[i] == '\t') {
			bool found = false;
			for (int z = 0; z < TextLine::slashT_SpaceCounts - 1; z++) {
				j++;
				if (j == visibleIndex) {
					found = true;
					break;
				}
			}
			if (found) {
				break;
			}
		}
	}
	return i;
}


