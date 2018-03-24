#include "InputForm.h"


int main() {
	HANDLE wHndl = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE rHndl = GetStdHandle(STD_INPUT_HANDLE);
	Color one = Color(Red << 4 | Green);
	Color two = Color(Black << 4 | Yellow);
	InputForm first("", defaultDeactiveColor);
	first.setActiveColor(defaultActiveColor);
	first.setDeactiveColor(defaultDeactiveColor);
	first.setPosition({ 1, 1 });
	first.appearOnConsole(wHndl);
	DWORD numEventsRead = 0;
	INPUT_RECORD eventsBuffer[128];
	SetConsoleMode(rHndl, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	while (true) {
		ReadConsoleInput(rHndl, eventsBuffer, 128, &numEventsRead);
		for (int i = 0; i < numEventsRead; i++) {
			if (eventsBuffer[i].EventType == MOUSE_EVENT) {
				if (eventsBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
					if (first.isGettingInput()) {
						first.changeState();
					}
					if (first.isMouseOnButton(eventsBuffer[i])) {
						first.changeState();
					}
				}
			}
			else if (eventsBuffer[i].EventType == KEY_EVENT) {
				if (eventsBuffer[i].Event.KeyEvent.bKeyDown) {
					if (first.isGettingInput()) {
						first.takeInput(eventsBuffer[i], wHndl);
					}
				}
			}
		first.appearOnConsole(wHndl);
		}
	}
	
}