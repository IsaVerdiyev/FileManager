#include "TextLine.h"
#include <iostream>

const int TextLine::slashT_SpaceCounts = 4;

TextLine::TextLine() : TextLine("")  {}

TextLine::TextLine(const std::string &s, Color c) : 
	isOnScreen{ false },
	positionChanged {true}
	{
	setTextAndColor(s, c);
	//setMinLengthArray();
}


void TextLine::setSymbols() {
	slashT_positions.clear();
	std::string s;
	for (int i = 0; i < textInLine.size(); i++) {
		if (textInLine[i] == '\t') {
			slashT_positions.push_back(i);
			for (int i = 0; i < slashT_SpaceCounts; i++) {
				s.push_back(' ');
			}
		}
		else {
			s.push_back(textInLine[i]);
		}
	}
	while (true) {
		try {
			sentenceSymbols.resize(s.size() > minLength ? s.size() : minLength);
			break;
		}
		catch (std::exception ex) {
			//try to resize while success
		}
	}
	int i = 0;
	for (; i < s.size(); i++) {
		sentenceSymbols[i].Char.AsciiChar = s[i];
	}
	for (; i < sentenceSymbols.size(); i++) {
		sentenceSymbols[i].Char.AsciiChar = ' ';
	}
	stringSize = s.size();
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
}

void TextLine::setTextAndColor(const std::string &s, Color c) {
	textInLine = s;
	lengthChanged = true;
	setSymbols();
	setColor(c);
}


std::vector<CHAR_INFO> &TextLine::getSentenceSymbols() {
	return sentenceSymbols;
}

void TextLine::setMinLength(int length) {
	lengthChanged = true;
	minLength = length;
	resizeAccordingToMinLength();
	
}

void TextLine::resizeAccordingToMinLength() {
	if (minLength > stringSize && minLength < sentenceSymbols.size()) {
		while (minLength < sentenceSymbols.size()) {
			sentenceSymbols.erase(sentenceSymbols.end() - 1);
		}
	}
	while(minLength > sentenceSymbols.size()) {
		CHAR_INFO c;
		c.Char.AsciiChar = ' ';
		c.Attributes = sentenceSymbols[0].Attributes;
		sentenceSymbols.push_back(c);
	}
}


void TextLine::createEraseArray(HANDLE &hndl) {
	eraseArray.clear();
	eraseArray.resize(getEndPosition().X - startPosition.X);
	SMALL_RECT readArea{ startPosition.X, startPosition.Y, getEndPosition().X - 1, getEndPosition().Y };
	ReadConsoleOutput(hndl, &eraseArray[0], { static_cast<short>(sentenceSymbols.size()) , 1 }, { 0, 0 }, &readArea);
}

void TextLine::putCharInfoArrayInConsoleBuffer(HANDLE &hndl, std::vector<CHAR_INFO> &symbolsArray, COORD pos) {
	SMALL_RECT writeArea{ pos.X, pos.Y, static_cast<short>(pos.X + symbolsArray.size() - 1), pos.Y };
	COORD bufferSize{ static_cast<short>(symbolsArray.size()), 1 };
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
	putCharInfoArrayInConsoleBuffer(hndl, sentenceSymbols, startPosition);
	isOnScreen = true;
}

void TextLine::appearOnConsoleScreen(HANDLE &hndl, COORD beginningPosition) {
	setPosition(beginningPosition);
	appearOnConsoleScreen(hndl);
}



COORD TextLine::getEndPosition() {
	return COORD{ static_cast<short>(startPosition.X + sentenceSymbols.size()), static_cast<short>(startPosition.Y) };
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
	/*std::string sentence;
	for (int i = 0; i < stringSize; i++) {
		sentence.push_back(sentenceSymbols[i].Char.AsciiChar);
	}
	return sentence;*/
	return textInLine;
}


