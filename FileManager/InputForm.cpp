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
	InputForm temp = *this;
	if(event.Event.KeyEvent.wVirtualKeyCode == VK_BACK) {
		if(temp.getCursorIndexPosition() != 0) {
			try {
				if ( !temp.isNextAfterIndentation(temp.getCursorIndexPosition())) {
					temp.getSentenceSymbols().erase(temp.getSentenceSymbols().begin() + temp.getCursorIndexPosition() - 1);
					temp.setCursorPositionIndex(temp.getCursorIndexPosition() - 1);
				}
			}
			catch (isNextAfterIndentationException exc) {
				temp.removeIndentation(exc.getIndex());
				temp.setCursorPositionIndex(temp.getCursorIndexPosition() - 4);
			}
			//catch (isOnIndentationException exc) {
			//	// need to add functionality===================

			//}
			
		}
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
		temp.turnInputStateOff();
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_LEFT) {
		if (temp.getCursorIndexPosition() != 0) {
			temp.setCursorPositionIndex(temp.getCursorIndexPosition() - 1);
		}
	}
	else if (event.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) {
		if (temp.getCursorIndexPosition() != temp.getSentenceSymbols().size() - 1) {
			try {
				if (!temp.isOnIndentation(temp.getCursorIndexPosition() + 1)) {
					temp.setCursorPositionIndex(temp.getCursorIndexPosition() + 1);
				}
			}
			catch (isOnIndentationException exc) {
				int resultCursorPositionIndex = exc.getIndex() * 3 + temp.slashT_positions[exc.getIndex()] + TextLine::slashT_SpaceCounts;
				temp.setCursorPositionIndex(resultCursorPositionIndex);
			}
		}
	}
	else {
		if (temp.cursorPositionIndex != temp.sentenceSymbols.size() - 1
			&& temp.sentenceSymbols[temp.sentenceSymbols.size() - 1].Char.AsciiChar == ' '
 			&& temp.sentenceSymbols.size() == temp.minLength) 
		{
			temp.sentenceSymbols.erase(temp.sentenceSymbols.end() - 1);
		}
		CHAR_INFO c;
		c.Char.AsciiChar = event.Event.KeyEvent.uChar.AsciiChar;
		c.Attributes = activeColor;
		temp.getSentenceSymbols().insert(temp.getSentenceSymbols().begin() + temp.cursorPositionIndex, c);

		temp.setCursorPositionIndex(temp.cursorPositionIndex + 1);
		//setVisibleStringSize();
	}
	temp.setVisibleStringSize();
	temp.resizeAccordingToMinLength();
	/*if (cursorPositionIndex >= sentenceSymbols.size()) {
		cursorPositionIndex = sentenceSymbols.size() - 1;
	}*/
	temp.setStringAfterFinishingInput();
	std::string resultString = temp.getTextInLine();
	*this = temp;
	this->setTextAndColor(resultString);
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
			//setCursorPositionIndex(index);
		}
	}
	catch (isOnIndentationException exc) {
		int resultIndexOfCursor = exc.getIndex() * (slashT_SpaceCounts - 1) + slashT_positions[exc.getIndex()];
		setCursorPositionIndex(resultIndexOfCursor);
	}
}

void InputForm::setVisibleStringSize() {
	int counter = sentenceSymbols.size() - 1;
	while (counter >= 0) {
		if (sentenceSymbols[counter].Char.AsciiChar == ' ') {
			counter--;
		}
		else {
			break;
		}
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
		stringWithSlashT.erase(stringWithSlashT.begin() + slashT_positions[i], stringWithSlashT.begin() + slashT_positions[i] + TextLine::slashT_SpaceCounts);
		stringWithSlashT.insert(stringWithSlashT.begin() + slashT_positions[i], '\t');
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
		for (int j = 1; j < TextLine::slashT_SpaceCounts; j++) {
			if (i * 3 + slashT_positions[i] + j == index) {
				throw isOnIndentationException(i);
			}
		}
	}
	return false;
}

bool InputForm::isNextAfterIndentation(int index) {
	std::string stringWithoutSlashIndentation = HelperFunctions::getStringWithReplacedSlashT_ToSpaces(textInLine, TextLine::slashT_SpaceCounts);
	for (int i = 0; i < slashT_positions.size(); i++) {
		if (i * 3 + slashT_positions[i] + TextLine::slashT_SpaceCounts == index) {
			throw isNextAfterIndentationException(i);
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


