#include "InputForm.h"
#include "ChoosableButton.h"


int main() {
	HANDLE wHndl = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE rHndl = GetStdHandle(STD_INPUT_HANDLE);
	Color one = Color(Red << 4 | Green);
	Color two = Color(Black << 4 | Yellow);
	ChoosableButton first("This is my sentence");
	
	//first.setTextAndColor("this is experiment");
	//first.setTextAndColor("");
	first.setPosition({ 1, 1 });
	first.appearOnConsole(wHndl);
	DWORD numEventsRead = 0;
	INPUT_RECORD eventsBuffer[128];
	SetConsoleMode(rHndl, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	while (true) {
		ReadConsoleInput(rHndl, eventsBuffer, 128, &numEventsRead);
		for (int i = 0; i < numEventsRead; i++) {
			if (eventsBuffer[i].EventType == MOUSE_EVENT) {
				if (first.isMouseOnButton(eventsBuffer[i])) {
					if (!first.isHover()) {
						first.setHoverState(true);
						first.appearOnConsole(wHndl);
					}
					if (eventsBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
						first.changeChosenState();
						first.appearOnConsole(wHndl);
					}
				}
				else {
					if (first.isHover()) {
						first.setHoverState(false);
						first.appearOnConsole(wHndl);
					}
				}
			}
		
		}
	}
	
}