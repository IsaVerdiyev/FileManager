#include "TextLine.h"
#include <iostream>


TextLine::TextLine() : TextLine("")  {}

TextLine::TextLine(const std::string &s, Color c) : 
	isOnScreen{ false },
	positionChanged {true}
	{
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
	positionChanged = true;
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
	lengthChanged = true;
	setSymbols(s);
	setColor(c);
}


std::vector<CHAR_INFO> &TextLine::getText() {
	return sentenceSymbols;
}

void TextLine::setMinLength(int length) {
	lengthChanged = true;
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


void TextLine::createEraseArray(HANDLE &hndl) {
	eraseArray.clear();
	eraseArray.resize(getEndPosition().X - startPosition.X);
	SMALL_RECT readArea{ startPosition.X, startPosition.Y, getEndPosition().X - 1, getEndPosition().Y };
	ReadConsoleOutput(hndl, &eraseArray[0], { static_cast<short>(sentenceSymbols.size() > minLength ? sentenceSymbols.size() : minLength), 1 }, { 0, 0 }, &readArea);
}

void TextLine::putCharInfoArrayInConsoleBuffer(HANDLE &hndl, std::vector<CHAR_INFO> &symbolsArray, COORD pos) {
	SMALL_RECT writeArea{ pos.X, pos.Y, pos.X + symbolsArray.size() - 1, pos.Y };
	COORD bufferSize{ symbolsArray.size(), 1 };
	if (bufferSize.X) {
		WriteConsoleOutputA(hndl, &symbolsArray[0], bufferSize, { 0, 0 }, &writeArea);
	}
}

void TextLine::appearOnConsoleScreen(HANDLE &hndl) {
	if (lengthChanged || positionChanged) {
		removeFromConsoleScreen(hndl);
		oldStartPosition = startPosition;
		createEraseArray(hndl);
		positionChanged = false;
		lengthChanged = false;
	}
	if (minLengthArray.size() > sentenceSymbols.size()) {
		putCharInfoArrayInConsoleBuffer(hndl, minLengthArray, startPosition);
	}
	putCharInfoArrayInConsoleBuffer(hndl, sentenceSymbols, startPosition);
	isOnScreen = true;
}

void TextLine::appearOnConsoleScreen(HANDLE &hndl, COORD beginningPosition) {
	setPosition(beginningPosition);
	appearOnConsoleScreen(hndl);
}



COORD TextLine::getEndPosition() {
	return COORD{ (short)(startPosition.X + (sentenceSymbols.size() > minLength ? sentenceSymbols.size() : minLength)), (short)startPosition.Y };
}

bool TextLine::isMouseOnButton(const INPUT_RECORD &event) {
	return event.Event.MouseEvent.dwMousePosition.X >= startPosition.X
		&& event.Event.MouseEvent.dwMousePosition.X <= getEndPosition().X
		&& event.Event.MouseEvent.dwMousePosition.Y >= startPosition.Y
		&& event.Event.MouseEvent.dwMousePosition.Y <= getEndPosition().Y;
}

void TextLine::removeFromConsoleScreen(HANDLE &hndl) {
	if (isOnScreen) {
		putCharInfoArrayInConsoleBuffer(hndl, eraseArray, oldStartPosition);
		isOnScreen = false;
	}
}

bool TextLine::getIsOnScreenState() {
	return isOnScreen;
}

int TextLine::getMinLength() {
	return minLength;
}

COORD TextLine::getStartPosition() {
	return startPosition;
}

std::string TextLine::getTextString() {
	std::string sentence;
	for (int i = 0; i < sentenceSymbols.size() - 1; i++) {
		sentence.push_back(sentenceSymbols[i].Char.AsciiChar);
	}
	return sentence;
}


