#ifndef CB
#define CB

#include "TextLine.h"

class ChoosableButton : public TextLine {
public:
	ChoosableButton();
	ChoosableButton(const std::string &s, Color c = SameColor);
	bool isMouseOnButton(const INPUT_RECORD &event);
	COORD getEndPosition();
};

#endif CB