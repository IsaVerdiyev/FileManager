#ifndef CB
#define CB

#include "TextLine.h"

class ChoosableButton : public TextLine {
	bool chosenState = false;
	bool hoverState = false;
public:
	Color buttonColor;
	Color chosenButtonColor;
	Color hoverColor;
	Color hoverOnChosenColor;
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
	void appearOnConsole(HANDLE &hndl) override;
};

#endif CB