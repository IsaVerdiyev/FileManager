#include "ChoosableButton.h"



ChoosableButton::ChoosableButton() : 
	buttonColor{ defaultButtonColor }, 
	chosenButtonColor {defaultChosenButtonColor},
	hoverColor {defaultHoverColor}
{}

ChoosableButton::ChoosableButton(const std::string &s, Color c) : 
	TextLine(s, c),
	buttonColor{ defaultButtonColor },
	chosenButtonColor{ defaultChosenButtonColor },
	hoverColor{ defaultHoverColor }
{}

COORD ChoosableButton::getEndPosition() {
	return COORD{ (short)(startPosition.X + sentenceSymbols.size()), (short)startPosition.Y };
}

bool ChoosableButton::isMouseOnButton(const INPUT_RECORD &event) {
	return event.Event.MouseEvent.dwMousePosition.X >= startPosition.X
		&& event.Event.MouseEvent.dwMousePosition.X <= getEndPosition().X 
		&& event.Event.MouseEvent.dwMousePosition.Y >= startPosition.X 
		&& event.Event.MouseEvent.dwMousePosition.Y <= getEndPosition().Y;
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