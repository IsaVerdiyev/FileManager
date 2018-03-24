#include "TextLine.h"
#include <iostream>


TextLine::TextLine() : TextLine("")  {}

TextLine::TextLine(const std::string &s, Color c) {
	setTextAndColor(s, c);
	setMinLengthArray();
}


void TextLine::setSymbols(const std::string &s) {
	while (true) {
		try {
			sentenceSymbols.resize(s.size() + 1);
			break;
		}
		catch (std::exception ex) {
			//try to resize while success
		}
	}
	for (int i = 0; i < sentenceSymbols.size(); i++) {
		sentenceSymbols[i].Char.AsciiChar = s[i];
	}
}

void TextLine::setPosition(COORD pos) {
	startPosition = pos;
}

void TextLine::setColor(Color c) {
	if (c == SameColor) {
		c = static_cast<Color>(sentenceSymbols[0].Attributes);
	}
	for (CHAR_INFO &symbol : sentenceSymbols) {
		symbol.Attributes = c;
	}
	setColorOfMinArray(c);
}

void TextLine::setTextAndColor(const std::string &s, Color c) {
	setSymbols(s);
	setColor(c);
}


std::vector<CHAR_INFO> &TextLine::getText() {
	return sentenceSymbols;
}

void TextLine::setMinLength(int length) {
	minLength = length;
	setMinLengthArray();
}

void TextLine::setMinLengthArray() {
	minLengthArray.clear();
	CHAR_INFO c;
	c.Char.AsciiChar = 0;
	for (int i = 0; i < minLength; i++) {
		minLengthArray.push_back(c);
	}
	setColorOfMinArray();
}

void TextLine::setColorOfMinArray(Color c) {
	if (c == SameColor) {
		c = static_cast<Color>(sentenceSymbols[0].Attributes);
	}
	for (CHAR_INFO &symbol : minLengthArray) {
		symbol.Attributes = c;
	}
}


void TextLine::createEraseArray() {
	oldStartPosition = startPosition;
	eraseArray.clear();
	CHAR_INFO c;
	c.Attributes = defaultBackground;
	c.Char.AsciiChar = NULL;
	for (int i = 0; i < ((sentenceSymbols.size() > minLengthArray.size()) ? sentenceSymbols.size() : minLengthArray.size()); i++) {
		eraseArray.push_back(c);
	}
}

void TextLine::putCharInfoArrayInConsoleBuffer(HANDLE &hndl, std::vector<CHAR_INFO> &symbolsArray, COORD pos) {
	SMALL_RECT writeArea{ pos.X, pos.Y, pos.X + symbolsArray.size() - 1, pos.Y };
	COORD bufferSize{ symbolsArray.size(), 1 };
	if (bufferSize.X) {
		WriteConsoleOutputA(hndl, &symbolsArray[0], bufferSize, { 0, 0 }, &writeArea);
	}
}

void TextLine::appearOnConsole(HANDLE &hndl) {
	putCharInfoArrayInConsoleBuffer(hndl, eraseArray, oldStartPosition);
	createEraseArray();
	if (minLengthArray.size() > sentenceSymbols.size()) {
		putCharInfoArrayInConsoleBuffer(hndl, minLengthArray, startPosition);
	}
	putCharInfoArrayInConsoleBuffer(hndl, sentenceSymbols, startPosition);
}

void TextLine::appearOnConsole(HANDLE &hndl, COORD beginningPosition) {
	setPosition(beginningPosition);
	appearOnConsole(hndl);
}

COORD TextLine::getEndPosition() {
	return COORD{ (short)(startPosition.X + (sentenceSymbols.size() > minLength ? sentenceSymbols.size() : minLength)), (short)startPosition.Y };
}

bool TextLine::isMouseOnButton(const INPUT_RECORD &event) {
	return event.Event.MouseEvent.dwMousePosition.X >= startPosition.X
		&& event.Event.MouseEvent.dwMousePosition.X <= getEndPosition().X
		&& event.Event.MouseEvent.dwMousePosition.Y >= startPosition.X
		&& event.Event.MouseEvent.dwMousePosition.Y <= getEndPosition().Y;
}
