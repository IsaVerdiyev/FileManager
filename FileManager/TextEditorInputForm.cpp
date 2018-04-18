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
			//try {

			/*if ( !temp.isNextAfterIndentation(temp.getCursorIndexPosition())) {
			temp.getSentenceSymbols().erase(temp.getSentenceSymbols().begin() + temp.getCursorIndexPosition() - 1);
			temp.setCursorPositionIndex(temp.getCursorIndexPosition() - 1);
			}
			}
			catch (isNextAfterIndentationException exc) {
			temp.removeIndentation(exc.getIndex());
			temp.setCursorPositionIndex(temp.getCursorIndexPosition() - 4);
			}*/
			//catch (isOnIndentationException exc) {
			//	// need to add functionality===================

			//}

			//}
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
		if (c.Char.AsciiChar == '\t') {
			CHAR_INFO space;
			space.Attributes = activeColor;
			space.Char.AsciiChar = ' ';
			for (int i = 0; i < TextLine::slashT_SpaceCounts - 1; i++) {
				getSentenceSymbols().insert(getSentenceSymbols().begin() + cursorPositionIndex, space);
			}
			cursorPositionIndex += 3;
		}
		std::string newTextInLine = getStringWithSlashT();
		setTextAndColor(newTextInLine);
	}
}