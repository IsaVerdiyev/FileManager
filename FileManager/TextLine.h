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
	void setSymbols(const std::string &s);
public:
	TextLine();
	TextLine(const std::string &s, Color c = SameColor);
	//Sentence(const Sentence &sent);
	//Sentence(Sentence &&sent);
	void setColor(Color c = SameColor);
	void setPosition(COORD pos);
	void appearOnConsole(HANDLE &hndl, COORD beginningPosition, Color Col =  SameColor);
	void appearOnConsole(HANDLE &hndl, Color Col = SameColor);
	void setTextAndColor(const std::string &s, Color c = SameColor);
	std::vector<CHAR_INFO> &getText();
};


#endif // !TL

