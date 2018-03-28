#include "Program.h"
#pragma warning(disable : 4996);


#include <iostream>//temp===============

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
	diskOptions( std::vector<std::string>{"Open", "Size"} ),
	path(""),
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
		disks.push_back( szDisk );
		//disks[disks.size() - 1].erase(disks[disks.size() - 1].end() - 1);
		disks[disks.size() - 1].erase(disks[disks.size() - 1].end() - 1);

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
				checkKeyEvent(eventsBuffer[i]);
			}
		}
	}
}

void Program::checkMouseEvent(INPUT_RECORD &event) {
	if (activePart == FILES) {
		performFilesPartEvents(event);
	}
	else if (activePart == OPTIONS) {
		performOptionsEvents(event);
	}
}

void Program::checkKeyEvent(INPUT_RECORD &event) {
	if (event.Event.KeyEvent.wVirtualKeyCode == VK_CONTROL && event.Event.KeyEvent.bKeyDown) {
		CTRLisPressed = true;
	}
	else if (!event.Event.KeyEvent.bKeyDown) {
		CTRLisPressed = false;
	}
}


void Program::setNewPath(int index) {
	std::string addedPath = items.getMenuItemStrings()[index];
	if (addedPath == "..") {
		size_t pos = path.find_last_of('/');
		if (pos == std::string::npos) {
			path = "";
		}
		else {
			path.erase(pos);
		}
	}
	else {
		if (path != "") {
			path += "/" + addedPath;
		}
		else {
			path += addedPath;
		}
	}
}

void Program::openFolder(int index) {
	setNewPath(index);
	items.removeMenuFromScreen(outputHandle);
	items.setMenuItems(getNewItemStringsFromNewPath());
	items.drawMenu(outputHandle);
}

std::vector<std::string> Program::getNewItemStringsFromNewPath() {
	std::vector<std::string> newItemNameStrings;
	std::string searchedPath(path + "/*");
	newItemNameStrings = getFiles(searchedPath);
	return newItemNameStrings;
}

std::vector<std::string> Program::getFiles(std::string searchedPath) {
	if (searchedPath == "/*") {
		return disks;
	}
	std::vector<std::string> ar;

	_finddata_t fileinfo;
	intptr_t done;
	done = _findfirst(searchedPath.c_str(), &fileinfo);
	if (done == -1) {
		_findclose(done);
		return ar;
	}
	do {
		if (!(fileinfo.attrib & _A_SYSTEM) && strcmp(".", fileinfo.name) && strcmp(fileinfo.name, "..") ) {
			ar.push_back(std::string(fileinfo.name));
		}
	} while (_findnext(done, &fileinfo) == 0);
	_findclose(done);
	if (searchedPath != "") {
		ar.insert(ar.begin(), "..");
	}
	return ar;
}

void Program::performFilesPartEvents(INPUT_RECORD &event) {
	for (int i = 0; i < items.getButtons().size(); i++) {
		if (items.getButtons()[i].isMouseOnButton(event)) {
			items.getButtons()[i].setHoverState(true);
			if (event.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
				if (!items.getButtons()[i].getChosenState()) {
					items.getButtons()[i].changeChosenState();
				}
				else {
					if (CTRLisPressed) {
						items.getButtons()[i].changeChosenState();
					}
					else if (HelperFunctions::is_dir((path + "/" + items.getMenuItemStrings()[i]).c_str())) {
						openFolder(i);
					}
					continue;
				}
			}
			else if (event.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
				activePart = OPTIONS;
				if (path == "") {
					diskOptions.setStartPosition(COORD{ event.Event.MouseEvent.dwMousePosition.X, event.Event.MouseEvent.dwMousePosition.Y });
					diskOptions.drawMenu(outputHandle);
					break;
				}
				else {
					options.setStartPosition(COORD{ event.Event.MouseEvent.dwMousePosition.X, event.Event.MouseEvent.dwMousePosition.Y });
					options.drawMenu(outputHandle);
					break;
				}
			}
		}
		else {
			items.getButtons()[i].setHoverState(false);
			if (!CTRLisPressed  && event.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
				if (items.getButtons()[i].getChosenState()) {
					items.getButtons()[i].changeChosenState();
				}
			}
		}
		items.drawMenu(outputHandle);
	}
}

void Program::performOptionsEvents(INPUT_RECORD &event) {
	activePart = FILES;
	if (path == "") {
		for (int i = 0; i < diskOptions.getButtons().size(); i++) {
			if (diskOptions.getButtons()[i].isMouseOnButton(event)) {
				activePart = OPTIONS;
			}
		}
		if (activePart != OPTIONS) {
			diskOptions.removeMenuFromScreen(outputHandle);
		}
	}
	else {
		for (int i = 0; i < options.getButtons().size(); i++) {
			if (options.getButtons()[i].isMouseOnButton(event)) {
				activePart = OPTIONS;
			}
		}
		if (activePart != OPTIONS) {
			options.removeMenuFromScreen(outputHandle);
		}
	}
}