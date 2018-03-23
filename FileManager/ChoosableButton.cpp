#include "ChoosableButton.h"


ChoosableButton::ChoosableButton() {}

ChoosableButton::ChoosableButton(const std::string &s, Color c) : TextLine(s, c) {}

COORD ChoosableButton::getEndPosition() {
	return COORD{ (short)(startPosition.X + sentenceSymbols.size()), (short)startPosition.Y };
}

bool ChoosableButton::isMouseOnButton(const INPUT_RECORD &event) {
	return event.Event.MouseEvent.dwMousePosition.X >= startPosition.X
		&& event.Event.MouseEvent.dwMousePosition.X <= getEndPosition().X 
		&& event.Event.MouseEvent.dwMousePosition.Y >= startPosition.X 
		&& event.Event.MouseEvent.dwMousePosition.Y <= getEndPosition().Y;
}