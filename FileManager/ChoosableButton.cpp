#include "ChoosableButton.h"



ChoosableButton::ChoosableButton() : 
	buttonColor{ defaultButtonColor }, 
	chosenButtonColor {defaultChosenButtonColor},
	hoverColor {defaultHoverColor},
	hoverOnChosenColor{ defaultHoverOnChosenColor }
{
	setColor(buttonColor);
}

ChoosableButton::ChoosableButton(const std::string &s) : 
	TextLine(s),
	buttonColor{ defaultButtonColor },
	chosenButtonColor{ defaultChosenButtonColor },
	hoverColor{ defaultHoverColor },
	hoverOnChosenColor{ defaultHoverOnChosenColor }
{
	setColor(buttonColor);
}





void ChoosableButton::setButtonColor(Color c) {
	buttonColor = c;
}
void ChoosableButton::setChosenButtonColor(Color c) {
	chosenButtonColor = c;
}
void ChoosableButton::setHovercolor(Color c) {
	hoverColor = c;
}

void ChoosableButton::setHoverOnChosenColor(Color c) {
	hoverOnChosenColor = c;
}

void ChoosableButton::changeChosenState() {
	if (isChosen()) {
		chosenState = false;
	}
	else {
		chosenState = true;
	}
}

void ChoosableButton::setHoverState(bool s) {
	hoverState = s;
}

bool ChoosableButton::isChosen() {
	return chosenState;
}
bool ChoosableButton::isHover() {
	return hoverState;
}

void ChoosableButton::appearOnConsole(HANDLE &hndl) {
	if (chosenState && hoverState) {
		setColor(hoverOnChosenColor);
	}
	else if (chosenState) {
		setColor(chosenButtonColor);
	}
	else if (hoverState) {
		setColor(hoverColor);
	}
	else {
		setColor(buttonColor);
	}
	TextLine::appearOnConsole(hndl);
}