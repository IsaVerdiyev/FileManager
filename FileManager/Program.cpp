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
	error.setColor(static_cast<Color>(Red << 4 | White));
	error.setPosition({ 90, 28 });
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
	items.setMenuItems(getContentOfFolder(path));
}


//std::vector<std::string> Program::getFiles(std::string searchedPath) {
//	if (searchedPath == "/*") {
//		return disks;
//	}
//	std::vector<std::string> ar;
//
//	_finddata_t fileinfo;
//	intptr_t done;
//	done = _findfirst(searchedPath.c_str(), &fileinfo);
//	if (done == -1) {
//		_findclose(done);
//		return ar;
//	}
//	do {
//		if (!(fileinfo.attrib & _A_SYSTEM) && strcmp(".", fileinfo.name) && strcmp(fileinfo.name, "..") ) {
//			ar.push_back(std::string(fileinfo.name));
//		}
//	} while (_findnext(done, &fileinfo) == 0);
//	_findclose(done);
//	if (searchedPath != "") {
//		ar.insert(ar.begin(), "..");
//	}
//	return ar;
//}

void Program::performFilesPartEvents(INPUT_RECORD &event) {
	itemsDrawing = false;
	optionsDrawing = false;
	errorDrawing = false;
	mouseClicked = false;
	for (int i = 0; i < items.getButtons().size(); i++) {
		if (items.getButtons()[i].isMouseOnButton(event)) {
			if (items.getButtons()[i].turnHoverOn()) {
				itemsDrawing = true;
			}
			if (event.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
				mouseClicked = true;
				handleFilesPartEventsWhenLeftMouseButtonPressed(items.getButtons()[i], event);
			}
			else if (event.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
				mouseClicked = true;
				handleEventsWhenRightMouseButtonPressedOfFilesPart(items.getButtons()[i], event);
			}
		}
		else {
			handleEventsWhenMouseIsNotOnItems(items.getButtons()[i], event);
		}
	}
	drawItemsAccordingToStates();
}

void Program::performOptionsEvents(INPUT_RECORD &event) {
	activePart = FILES;
	Menu *pointerToOptionsMenu;
	drawOptions = false;
	errorDrawing = false;
	mouseClicked = false;
	if (path == "") {
		pointerToOptionsMenu = &diskOptions;
	}
	else {
		pointerToOptionsMenu = &options;
	}
	for (int i = 0; i < pointerToOptionsMenu->getButtons().size(); i++) {
		if (pointerToOptionsMenu->getButtons()[i].isMouseOnButton(event)) {
			activePart = OPTIONS;
			if (pointerToOptionsMenu->getButtons()[i].turnHoverOn()) {
					drawOptions = true;
			}
			if (event.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
				mouseClicked = true;
				if (i == OPEN) {
					std::string searchedPath = getNewPath(chosenButtons[chosenButtons.size() - 1]);
					if (fs::is_directory(searchedPath) || searchedPath == "") {
						pointerToOptionsMenu->removeMenuFromScreen(outputHandle);
						openFolder(chosenButtons[chosenButtons.size() - 1]);
						activePart = FILES;
					}
					else {
						error.setTextAndColor(isNotFolderError);
						errorDrawing = true;
					}
				}
				else if (pointerToOptionsMenu == &options) {
					if (i == RENAME) {

					}
				}
			}
			else if (event.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
				mouseClicked = true;
			}
		}
		else {
			pointerToOptionsMenu->getButtons()[i].turnHoverOff();
		}
	}
	if (mouseClicked && !errorDrawing) {
		error.removeFromConsoleScreen(outputHandle);
	}
	if (activePart != OPTIONS) {
		pointerToOptionsMenu->removeMenuFromScreen(outputHandle);
		items.drawMenu(outputHandle);
	}
	else {
		if (drawOptions) {
			pointerToOptionsMenu->drawMenu(outputHandle);
		}
	}
	if (errorDrawing) {
		error.appearOnConsoleScreen(outputHandle);
	}
	
}

void Program::handleFilesPartEventsWhenLeftMouseButtonPressed(ChoosableButton &button, INPUT_RECORD &event) {
	int index = &button - &items.getButtons()[0];
	if (button.turnChosenStateOn()) {
		chosenButtons.push_back(index);
		itemsDrawing = true;
	}
	else {
		if (CtrlisPressed) {
			button.turnChosenStateOff();
			chosenButtons.erase(std::remove(chosenButtons.begin(), chosenButtons.end(), index), chosenButtons.end());
		}
		else {
			std::string searchedPath = getNewPath(index);
			if (fs::is_directory(searchedPath.c_str()) || searchedPath == "") {
				openFolder(index);
			}
			else {
				error.setTextAndColor(isNotFolderError);
				errorDrawing = true;
			}
			itemsDrawing = true;
		}
	}
}

void Program::handleEventsWhenRightMouseButtonPressedOfFilesPart(ChoosableButton &button, INPUT_RECORD &event) {
	int index = &button - &items.getButtons()[0];
	if (button.turnChosenStateOn()) {
		chosenButtons.push_back(index);
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

void Program::drawItemsAccordingToStates() {
	if (!errorDrawing && mouseClicked) {
		error.removeFromConsoleScreen(outputHandle);
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
	if (errorDrawing) {
		error.appearOnConsoleScreen(outputHandle);
	}
}


void Program::handleEventsWhenMouseIsNotOnItems(ChoosableButton &button, INPUT_RECORD &event) {
	int index = &button - &items.getButtons()[0];
	if (button.turnHoverOff()) {
		itemsDrawing = true;
	}
	if (!CtrlisPressed && (event.Event.MouseEvent.dwButtonState & (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED))) {
		mouseClicked = true;
		if (button.turnChosenStateOff()) {
			chosenButtons.erase(std::remove(chosenButtons.begin(), chosenButtons.end(), index), chosenButtons.end());
			itemsDrawing = true;
		}
	}
}

std::vector<std::string> Program::getContentOfFolder(std::string path) {
	if (path == "") {
		return disks;
	}
	std::vector<std::string> ar;
	ar.push_back("..");
	for (auto &p : fs::directory_iterator(path)) {
		std::string itemName = p.path().filename().string();
		itemName += p.path().extension().string();
		ar.push_back(itemName);
	}
	return ar;
}
