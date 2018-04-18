#ifndef CB
#define CB

/*
	Class creates object of buttons that can be pressed, chosen and on which user can hover
	Class is inherited from TextLine class
*/

#include "TextLine.h"

class ChoosableButton : public TextLine {
	bool chosenState = false; // flag for showing if button is chosen or not
	bool hoverState = false; // flag for showing if button is hovered
	Color buttonColor; // common color when object is displayed
	Color chosenButtonColor; // color when object is chosen
	Color hoverColor; // color when object is hovered
	Color hoverOnChosenColor; // color when object is both hovered on and chosen
public:
	// Constructors calls TextLine constructs and set colors additionally
	ChoosableButton();
	ChoosableButton(const std::string &s);
	////////////////////////////////////

	void setButtonColor(Color c); // sets buttoncolor
	void setChosenButtonColor(Color c); // sets chosenButtonColor
	void setHovercolor(Color c); // sets hoverColor
	void setHoverOnChosenColor(Color c); // sets hoverOnChosenColor
	void changeChosenState(); // changes flag of chosenState
	void setHoverState(bool s); // sets flag of hoverState specified in parameter
	bool isChosen(); // returns chosenState
	bool isHover(); // returns hoverState
	void refreshStateColor(); // refreshes color of object according to specified states
	void appearOnConsoleScreen(HANDLE &hndl) override; // draws button
	bool turnHoverOn(); // sets hoverState true and returns true if hoverState changed
	bool turnHoverOff(); // sets hoverState false and returns true if hoverState changed
	bool turnChosenStateOn(); // sets chosenState true and returns true if chosenState changed
	bool turnChosenStateOff(); // sets chosenState false and returns true if chosenState changed
};

#endif CB