#include "TextLine.h"
#include <iostream>


TextLine::TextLine() : TextLine("")  {}

TextLine::TextLine(const std::string &s, Color c) {
	setTextAndColor(s, c);
}

void TextLine::appearOnConsole(HANDLE &hndl, COORD beginningPosition, Color c) {
	if (c != SameColor) {
		setColor(c);
	}
	SMALL_RECT writeArea{ beginningPosition.X, beginningPosition.Y, beginningPosition.X + sentenceSymbols.size() - 1, beginningPosition.Y };
	COORD bufferSize{ sentenceSymbols.size(), 1 };
	WriteConsoleOutputA(hndl, &sentenceSymbols[0],  bufferSize, { 0, 0 }, &writeArea);
}

void TextLine::setColor(Color c) {
	if (c == SameColor) {
		c = static_cast<Color>(sentenceSymbols[0].Attributes);
	}
	for (CHAR_INFO &symbol : sentenceSymbols) {
		symbol.Attributes = c;
	}
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

void TextLine::setTextAndColor(const std::string &s, Color c) {
	setSymbols(s);
	if (c == SameColor) {
		if (sentenceSymbols[0].Attributes != sentenceSymbols[sentenceSymbols.size() - 1].Attributes) {
			setColor();
		}
	}
	else {
		setColor(c);
	}
}

void TextLine::setPosition(COORD pos) {
	startPosition = pos;
}

void TextLine::appearOnConsole(HANDLE &hndl, Color c) {
	if (c != SameColor) {
		setColor(c);
	}
	SMALL_RECT writeArea{ startPosition.X, startPosition.Y, startPosition.X + sentenceSymbols.size() - 1, startPosition.Y };
	COORD bufferSize{ sentenceSymbols.size(), 1 };
	WriteConsoleOutputA(hndl, &sentenceSymbols[0], bufferSize, { 0, 0 }, &writeArea);
}

std::vector<CHAR_INFO> &TextLine::getText() {
	return sentenceSymbols;
}