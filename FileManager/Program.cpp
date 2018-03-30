#include "Program.h"
#pragma warning(disable : 4996);


#include <iostream>//temp===============

Program::Program() :
	options(std::vector<std::string> {
		"Open",
		"Rename",
		"Cut",
		"Copy",
		"Paste",
		"Size",
		"Create file",
		"Create folder",
		"Delete"
		}),
	diskOptions( std::vector<std::string>{"Open", "Size"} ),
	path(""),
	CtrlisPressed{false},
	activePart {FILES}
{
	setDisks();
	items.setMenuItems(disks);
	items.setStartPosition({ 5, 5 });
	options.setHoverColor(static_cast<Color>(Green << 4 | White));
	options.setStandardColor(static_cast<Color>(Red << 4 | White));
	diskOptions.setHoverColor(static_cast<Color>(Green << 4 | White));
	diskOptions.setStandardColor(static_cast<Color>(Red << 4 | White));
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
		CtrlisPressed = true;
	}
	else if (!event.Event.KeyEvent.bKeyDown) {
		CtrlisPressed = false;
	}
}


std::string Program::getNewPath(int index) {
	std::string searchedPath = path;
	std::string addedPath = items.getMenuItemStrings()[index];
	if (addedPath == "..") {
		size_t pos = searchedPath.find_last_of('/');
		if (pos == std::string::npos) {
			searchedPath = "";
		}
		else {
			searchedPath.erase(pos);
		}
	}
	else {
		if (searchedPath != "") {
			searchedPath += "/" + addedPath;
		}
		else {
			searchedPath += addedPath;
		}
	}
	return searchedPath;
}

void Program::openFolder(int index) {
	path = getNewPath(index);
	items.removeMenuFromScreen(outputHandle);
	items.setMenuItems(getNewItemStringsFromNewPath());
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
	bool itemsDrawing = false;
	bool optionsDrawing = false;
	for (int i = 0; i < items.getButtons().size(); i++) {
		if (items.getButtons()[i].isMouseOnButton(event)) {
			if (items.getButtons()[i].turnHoverOn()) {
				itemsDrawing = true;
			}
			if (event.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
				if (items.getButtons()[i].turnChosenStateOn()) {
					chosenButtons.push_back(i);
					itemsDrawing = true;
				}
				else {
					if (CtrlisPressed) {
						items.getButtons()[i].turnChosenStateOff();
						chosenButtons.erase(std::remove(chosenButtons.begin(), chosenButtons.end(), i), chosenButtons.end());
					}
					else {
						std::string searchedPath = getNewPath(i);
						if(sf::is_directory(searchedPath.c_str()) || searchedPath == "") {
						openFolder(i);
					}
					itemsDrawing = true;
				}
				}
			}
			else if (event.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
				if (items.getButtons()[i].turnChosenStateOn()) {
					chosenButtons.push_back(i);
				}
				activePart = OPTIONS;
				optionsDrawing = true;
				if (path == "") {
					diskOptions.setStartPosition(COORD{ event.Event.MouseEvent.dwMousePosition.X, event.Event.MouseEvent.dwMousePosition.Y });
				}
				else {
					options.setStartPosition(COORD{ event.Event.MouseEvent.dwMousePosition.X, event.Event.MouseEvent.dwMousePosition.Y });
				}
			}
		}
		else {
			if (items.getButtons()[i].turnHoverOff()) {
				itemsDrawing = true;
			}
			if (!CtrlisPressed  && (event.Event.MouseEvent.dwButtonState & (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED))) {
				if (items.getButtons()[i].turnChosenStateOff()) {
					chosenButtons.erase(std::remove(chosenButtons.begin(), chosenButtons.end(), i), chosenButtons.end());
					itemsDrawing = true;
				}
			}
		}
	}
	if (itemsDrawing) {
		items.drawMenu(outputHandle);
		itemsDrawing = false;
	}
	if (optionsDrawing) {
		if (path == "") {
			diskOptions.drawMenu(outputHandle);
		}
		else {
			options.drawMenu(outputHandle);
		}
		optionsDrawing = false;
	}
}

void Program::performOptionsEvents(INPUT_RECORD &event) {
	activePart = FILES;
	Menu *pointerToOptionsMenu;
	bool drawOptions = false;
	if (path == "") {
		pointerToOptionsMenu = &diskOptions;
	}
	else {
		pointerToOptionsMenu = &options;
	}
	for (int i = 0; i < pointerToOptionsMenu->getButtons().size(); i++) {
		if (pointerToOptionsMenu->getButtons()[i].isMouseOnButton(event)) {
			activePart = OPTIONS;
			if (!pointerToOptionsMenu->getButtons()[i].turnHoverOn()) {
					drawOptions = true;
			}
			if (event.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
				if (i == OPEN) {
					std::string searchedPath = getNewPath(chosenButtons[chosenButtons.size() - 1]);
					if (sf::is_directory(searchedPath) || searchedPath == "") {
						pointerToOptionsMenu->removeMenuFromScreen(outputHandle);
						openFolder(chosenButtons[chosenButtons.size() - 1]);
						activePart = FILES;
					}
				}
			}
		}
		else {
			pointerToOptionsMenu->getButtons()[i].turnHoverOff();
		}
	}
	if (activePart != OPTIONS) {
		pointerToOptionsMenu->removeMenuFromScreen(outputHandle);
		items.drawMenu(outputHandle);
	}
	else {
		pointerToOptionsMenu->drawMenu(outputHandle);
	}
	
}