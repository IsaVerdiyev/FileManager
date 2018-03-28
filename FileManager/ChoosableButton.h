#ifndef CB
#define CB

#include "TextLine.h"

class ChoosableButton : public TextLine {
	bool chosenState = false;
	bool hoverState = false;
	Color buttonColor;
	Color chosenButtonColor;
	Color hoverColor;
	Color hoverOnChosenColor;
public:
	ChoosableButton();
	ChoosableButton(const std::string &s);
	void setButtonColor(Color c);
	void setChosenButtonColor(Color c);
	void setHovercolor(Color c);
	void setHoverOnChosenColor(Color c);
	void changeChosenState();
	void setHoverState(bool s);
	bool isChosen();
	bool isHover();
	void refreshStateColor();
	void appearOnConsoleScreen(HANDLE &hndl) override;
	bool getChosenState();
	bool getHoverState();
	bool turnHoverOn();
	bool turnHoverOff();
	bool turnChosenStateOn();
	bool turnChosenStateOff();
};

#endif CB