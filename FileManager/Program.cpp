#include "Program.h"


Program::Program() :
	options(std::vector<std::string> {
		"Open",
		"Rename",
		"Move",
		"Copy",
		"Size",
		"Create file",
		"Create folder",
		"Delete"
		}),
	path("This PC"),
	CTRLisPressed{false},
	activePart {FILES}
{
	setDisks();
	items.setMenuItems(disks);
	items.setStartPosition({ 5, 5 });
	outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(inputHandle, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	items.drawMenu(outputHandle);
}


void Program::setDisks() {
	int sz = GetLogicalDriveStrings(NULL, 0);
	char* szLogicalDrives = new char[sz];
	GetLogicalDriveStrings(sz, szLogicalDrives);

	char *pointerToDrivesString = szLogicalDrives;
	while (*pointerToDrivesString)
	{
		char szDisk[80];
		lstrcpy(szDisk, pointerToDrivesString);
		disks.push_back({ szDisk });

		while (*pointerToDrivesString) {
			pointerToDrivesString++;
		}
		pointerToDrivesString++;
	}
	delete[] szLogicalDrives;
}

void Program::performCycleOfEvents() {
	bool isRunning = true;
	DWORD numEventsRead = 0;
	while (isRunning) {
		ReadConsoleInput(inputHandle, eventsBuffer, 128, &numEventsRead);
		for (int i = 0; i < numEventsRead; i++) {
			if (eventsBuffer[i].EventType == MOUSE_EVENT) {
				checkMouseEvent(eventsBuffer[i]);
			}
			else if (eventsBuffer[i].EventType == KEY_EVENT) {

			}
		}
		
	}
}

void Program::checkMouseEvent(INPUT_RECORD &event) {
	if (activePart == FILES) {
		for (int i = 0; i < items.getButtons().size(); i++) {
			if (items.getButtons()[i].isMouseOnButton(event)) {
				items.getButtons()[i].setHoverState(true);
				if (event.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
					if (!items.getButtons()[i].getChosenState()) {
						items.getButtons()[i].changeChosenState();
					}
				}
			}
			else {
				items.getButtons()[i].setHoverState(false);
				if (!CTRLisPressed) {
					if (items.getButtons()[i].getChosenState()) {
						items.getButtons()[i].changeChosenState();
					}
				}
			}
		}
		items.drawMenu(outputHandle);
	}
	else if (activePart == OPTIONS) {

	}
}