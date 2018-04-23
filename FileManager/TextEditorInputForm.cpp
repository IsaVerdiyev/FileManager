#include "TextEditorInputForm.h"


TextEditorInputForm::TextEditorInputForm(): InputForm() {}

TextEditorInputForm::TextEditorInputForm(const std::string &s)
	: InputForm(s) {}

void TextEditorInputForm::takeInput(const INPUT_RECORD &event) {
	lengthChanged = true;
	if (event.Event.KeyEvent.wVirtualKeyCode == VK_BACK) {
		if (getCursorIndexPosition() != 0) {
			if (getVisibleStringSize() >= getCursorIndexPosition()) {
				int indexInLine = findIndexInTextLine(cursorPositionIndex - 1);
				if (textInLine[indexInLine] == '\t') {
					cursorPositionIndex -= 4;
					setVisibleStringSize(stringSize - 4);
				}
				else {
					cursorPositionIndex--;
					setVisibleStringSize(stringSize - 1);
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
		if (cursorPositionIndex == sentenceSymbols.size() - 1) {
			return;
		}
		CHAR_INFO c;
		c.Char.AsciiChar = event.Event.KeyEvent.uChar.AsciiChar;
		c.Attributes = activeColor;
		getSentenceSymbols().insert(getSentenceSymbols().begin() + getCursorIndexPosition(), c);
		cursorPositionIndex++;
		setVisibleStringSize(stringSize + 1);
		if (c.Char.AsciiChar == '\t') {
			CHAR_INFO space;
			space.Attributes = activeColor;
			space.Char.AsciiChar = ' ';
			for (int i = 0; i < TextLine::slashT_SpaceCounts - 1; i++) {
				getSentenceSymbols().insert(getSentenceSymbols().begin() + cursorPositionIndex, space);
			}
			cursorPositionIndex += 3;
			setVisibleStringSize(stringSize + 3);
		}
		std::string newTextInLine = getStringWithSlashT();
		setTextAndColor(newTextInLine);
	}
}



void TextEditorInputForm::setCursorPositionIndex(int index) {
	try {
		if (!isOnIndentation(index)) {
			if (index < 0 || index > getVisibleStringSize()) {
				if (getVisibleStringSize()) {
					setCursorPositionIndex(getVisibleStringSize());
				}
				else {
					cursorPositionIndex = 0;
				}
			}
			else {
				cursorPositionIndex = index;
			}
		}
	}
	catch (isOnIndentationException exc) {
		setCursorPositionIndex(exc.getIndex());
	}
}




void TextEditorInputForm::setVisibleStringSize(int s) {
	stringSize = s;
}



std::string TextEditorInputForm::getVisibleString() {
	std::string visibleString;
	for (int i = 0; i < stringSize; i++) {
		visibleString.push_back(sentenceSymbols[i].Char.AsciiChar);
	}
	return visibleString;
}

size_t TextEditorInputForm::findIndexInTextLine(int visibleIndex) {
	std::string str = textInLine;
	size_t i = 0;
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
					return i;
				}
			}
			if (found) {
				break;
			}
		}
	}
	return i;
}



