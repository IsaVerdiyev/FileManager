#ifndef TL
#define TL

#include <string>
#include <vector>
#include <Windows.h>
#include "Colors.h"

class TextLine {
protected:
	COORD startPosition;
	std::vector<CHAR_INFO> sentenceSymbols;
	std::vector<CHAR_INFO> eraseArray;
	std::vector<CHAR_INFO> minLengthArray;
	COORD oldStartPosition;
	void setSymbols(const std::string &s);
	int minLength = 1;
	void setMinLengthArray();
	void createEraseArray();
	void setColorOfMinArray(Color c = SameColor);
public:
	TextLine();
	TextLine(const std::string &s, Color c = SameColor);
	void setColor(Color c = SameColor);
	void setPosition(COORD pos);
	void setTextAndColor(const std::string &s, Color c = SameColor);
	std::vector<CHAR_INFO> &getText();
	void setMinLength(int length);
	void putCharInfoArrayInConsoleBuffer(HANDLE &hndl, std::vector<CHAR_INFO> &symbolsArray, COORD pos);
	void appearOnConsole(HANDLE &hndl, COORD beginningPosition);
	virtual void appearOnConsole(HANDLE &hndl);
	COORD getEndPosition();
	bool isMouseOnButton(const INPUT_RECORD &event);
};


#endif // !TL

