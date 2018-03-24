#ifndef CB
#define CB

#include "TextLine.h"

class ChoosableButton : public TextLine {
public:
	Color buttonColor;
	Color chosenButtonColor;
	Color hoverColor;
	ChoosableButton();
	ChoosableButton(const std::string &s, Color c = SameColor);
	bool isMouseOnButton(const INPUT_RECORD &event);
	COORD getEndPosition();
	void setButtonColor(Color c);
	void setChosenButtonColor(Color c);
	void setHovercolor(Color c);
};

#endif CB