#include "ChoosableButton.h"


int main() {
	HANDLE wHndl = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE rHndl = GetStdHandle(STD_INPUT_HANDLE);
	Color one = Color(Red << 4 | Green);
	Color two = Color(Black << 4 | Yellow);
	ChoosableButton first("first", one);
	first.setPosition(COORD{ 20, 20 });
	first.appearOnConsole(wHndl);
	DWORD numEventsRead = 0;
	INPUT_RECORD eventsBuffer[128];
	SetConsoleMode(rHndl, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	while (true) {
		ReadConsoleInput(rHndl, eventsBuffer, 128, &numEventsRead);
		for (int i = 0; i < numEventsRead; i++) {
			if (eventsBuffer[i].EventType == MOUSE_EVENT) {
				if (first.isMouseOnButton(eventsBuffer[i])) {
					first.setColor(two);
				}
				else {
					first.setColor(one);
				}
			}
		first.appearOnConsole(wHndl);
		}
	}
	
}