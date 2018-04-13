#include "Program.h"
#pragma warning(disable : 4996);




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
		"Delete"}),
	diskOptions(std::vector<std::string>{"Open"}),
		path(""),
		CtrlisPressed{ false },
		activePart{ FILES }
{
	setDisks();
	items.setMenuItems(disks);
	items.setStartPosition({ 5, 8 });
	options.setHoverColor(static_cast<Color>(Green << 4 | White));
	options.setStandardColor(static_cast<Color>(Red << 4 | White));
	diskOptions.setHoverColor(static_cast<Color>(Green << 4 | White));
	diskOptions.setStandardColor(static_cast<Color>(Red << 4 | White));
	fileEditor.setStartPosition({ 5, 8 });
	outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(inputHandle, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	CONSOLE_SCREEN_BUFFER_INFO scrInfo;
	GetConsoleScreenBufferInfo(outputHandle, &scrInfo);
	scrInfo.dwSize.X = 2500;
	SetConsoleScreenBufferSize(outputHandle, scrInfo.dwSize);
	items.drawMenu(outputHandle);
	error.setColor(static_cast<Color>(Red << 4 | White));
	error.setPosition({ 90, 28 });
	info.setPosition({ 90, 15 });
	info.setColor(defaultColor);
	pointerToOptionsMenu = &diskOptions;
	searchPart.setStartPosition({ 5, 5 });
	searchPart.appearOnConsole(outputHandle);
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
			disks.push_back(szDisk);
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
			drawItemsAccordingToStates();
		}
	}

	void Program::checkMouseEvent(const INPUT_RECORD &event) {
		if (event.Event.MouseEvent.dwEventFlags & MOUSE_WHEELED) {
			scrollVertically(event);
		}
		else if (event.Event.MouseEvent.dwEventFlags & MOUSE_HWHEELED) {
			scrollHorizontally(event);
		}
		else if (activePart == FILES) {
			performFilesPartEvents(event);
		}
		else if (activePart == OPTIONS) {
			performOptionsEvents(event);
		}
		/*else if (activePart == SEARCH) {
			performSearchTableEvents(event);
		}*/
		else if (activePart == SEARCH_RESULTS) {
			performSearchResultsEvents(event);
		}
		else if (activePart == EDIT_FILE) {
			performTextEditingEvents(event);
		}
	}

	void Program::checkKeyEvent(const INPUT_RECORD &event) {
		if (event.Event.KeyEvent.wVirtualKeyCode == VK_CONTROL && event.Event.KeyEvent.bKeyDown) {
			CtrlisPressed = true;
		}
		else if (!event.Event.KeyEvent.bKeyDown) {
			CtrlisPressed = false;
		}
		if (activePart == FILES) {
			if (input.isGettingInput() && event.Event.KeyEvent.bKeyDown) {
				if ((event.Event.KeyEvent.wVirtualKeyCode >= 0x30 && event.Event.KeyEvent.wVirtualKeyCode <= 0xdf) || event.Event.KeyEvent.wVirtualKeyCode == VK_BACK || event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN || event.Event.KeyEvent.wVirtualKeyCode == VK_SPACE || event.Event.KeyEvent.wVirtualKeyCode == VK_LEFT || event.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) {
					input.takeInput(event);
				}
				if (event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
					input.turnInputStateOff();
					isRenameProcess = false;
				}
				doRenameOperations();
			}
		}
		if (searchPart.getSearchInput().isGettingInput() && event.Event.KeyEvent.bKeyDown) {
		
			if ((event.Event.KeyEvent.wVirtualKeyCode >= 0x30 && event.Event.KeyEvent.wVirtualKeyCode <= 0xdf) || event.Event.KeyEvent.wVirtualKeyCode == VK_BACK || event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN || event.Event.KeyEvent.wVirtualKeyCode == VK_SPACE || event.Event.KeyEvent.wVirtualKeyCode == VK_LEFT || event.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) {
				searchPart.getSearchInput().takeInput(event);
				searchPart.appearOnConsole(outputHandle);
			}
			if (event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
				searchPart.getSearchInput().turnInputStateOff();
				searchPart.appearOnConsole(outputHandle);
			}
			
		}
		if (activePart == EDIT_FILE) {
			textEditDrawing = false;
			if (fileEditor.getButtons()[fileEditor.getInputLineIndex()].isGettingInput()) {
				if ((event.Event.KeyEvent.wVirtualKeyCode >= 0x30 && event.Event.KeyEvent.wVirtualKeyCode <= 0xdf) || event.Event.KeyEvent.wVirtualKeyCode == VK_BACK || event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN || event.Event.KeyEvent.wVirtualKeyCode == VK_SPACE || event.Event.KeyEvent.wVirtualKeyCode == VK_LEFT || event.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) {
					fileEditor.getButtons()[fileEditor.getInputLineIndex()].takeInput(event);
					textEditDrawing = true;
				}
				/*if (event.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
					searchPart.getSearchInput().turnInputStateOff();
					searchPart.appearOnConsole(outputHandle);
				}*/
			}
		}
	}


	std::string Program::getNewPath(int index) {
		std::string searchedPath = path;
		std::string addedPath = items.getMenuItemStrings()[index];
		if (addedPath == "..") {
			size_t pos = searchedPath.find_last_of('\\');
			if (pos == std::string::npos) {
				searchedPath = "";
			}
			else {
				searchedPath.erase(pos);
			}
		}
		else {
			if (searchedPath != "") {
				searchedPath = searchedPath + "\\" + addedPath;
			}
			else {
				searchedPath += addedPath;
			}
		}
		return searchedPath;
	}

	std::string Program::getNewPath(const std::string &name) {
		std::string folder = items.getMenuItemStrings()[chosenButtons[chosenButtons.size() - 1]];
		std::string newPath;
		if (folder == "..") {
				newPath = path + "/" + name;
		}
		else {
			newPath = path + "/" + folder + "/" + name;
		}
		
		return newPath;
	}

	void Program::startOpeningProcess(const std::string &searchedPath) {
		if (fs::is_directory(searchedPath) || searchedPath == "") {
			path = searchedPath;
			openFolder(path);

		}
		else {
			std::string txt(".txt");
			std::string cpp(".cpp");
			std::string h(".h");
			if (!searchedPath.compare(searchedPath.size() - txt.size(), txt.size(), txt) ||
				!searchedPath.compare(searchedPath.size() - cpp.size(), cpp.size(), cpp) ||
				!searchedPath.compare(searchedPath.size() - h.size(), h.size(), h))  {
				std::ifstream file(searchedPath);
				if (file) {
					fileEditor.setMenuItems(HelperFunctions::getTextFromFile(file));
					activePart = EDIT_FILE;
					textEditDrawing = true;
					return;
				}
			}
			error.setTextAndColor(isNotFolderError);
			errorDrawing = true;
			
		}
	}

	void Program::openFolder(int index) {
		path = getNewPath(index);
		openFolder(path);		
	}

	void Program::openFolder(std::string path) {
		pointerToOptionsMenu->removeMenuFromScreen(outputHandle);
		items.removeMenuFromScreen(outputHandle);
		items.setMenuItems(getContentOfFolder(path));
		chosenButtons.clear();
		itemsDrawing = true;
		activePart = FILES;
	}

	

	void Program::performFilesPartEvents(const INPUT_RECORD &event) {
		itemsDrawing = false;
		optionsDrawing = false;
		errorDrawing = false;
		mouseClicked = false;
		infoDrawing = false;
		if (isRenameProcess) {
			performMouseEventsDuringRenameProcess(event);
		}
		performSearchTableEvents(event);
		if (!isRenameProcess && !searchPart.getSearchInput().isGettingInput()) {
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

		}
	}

	void Program::performMouseEventsDuringRenameProcess(const INPUT_RECORD &event) {
		if (FROM_LEFT_1ST_BUTTON_PRESSED & event.Event.MouseEvent.dwButtonState && input.isMouseOnButton(event)) {
			input.turnInputStateOn(event);
		}
		else if ((FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED) & event.Event.MouseEvent.dwButtonState && !input.isMouseOnButton(event)) {
			input.turnInputStateOff();
			isRenameProcess = false;
		}
		doRenameOperations();
	}

	void Program::performSearchTableEvents(const INPUT_RECORD &event) {
		if ((FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED) & event.Event.MouseEvent.dwButtonState) {
			mouseClicked = true;
			errorDrawing = false;
			infoDrawing = false;
		}
		searchResultsDrawing = false;
		if (searchPart.getSearchHeader().isMouseOnButton(event) && !searchPart.getSearchInput().isGettingInput()) {
			if (searchPart.getSearchHeader().turnHoverOn()) {
				searchPart.appearOnConsole(outputHandle);
			}
			if (event.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
				try {
					searchPart.getSearchResults().removeMenuFromScreen(outputHandle);
					searchPart.search(path, disks);
					activePart = SEARCH_RESULTS;
					searchResultsDrawing = true;
				}
				catch (std::exception ex) {
					error.setTextAndColor(ex.what());
					errorDrawing = true;
				}
			}
		}
		else {
			if (searchPart.getSearchHeader().turnHoverOff() && !searchPart.getSearchInput().isGettingInput()) {
				searchPart.appearOnConsole(outputHandle);
			}
		}
		if (event.Event.MouseEvent.dwButtonState & (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED) && !searchPart.getSearchInput().isMouseOnButton(event)) {
			if (searchPart.getSearchInput().turnInputStateOff()) {
				searchPart.appearOnConsole(outputHandle);
				activePart = FILES;
			}
		}
		else if (event.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED && searchPart.getSearchInput().isMouseOnButton(event)) {
			searchPart.getSearchInput().turnInputStateOn(event);
			searchPart.appearOnConsole(outputHandle);
			//activePart = SEARCH;
		}
	}

	void Program::performSearchResultsEvents(const INPUT_RECORD &event) {
		performSearchTableEvents(event);
		if (activePart == SEARCH_RESULTS) {
			for (int i = 0; i < searchPart.getSearchResults().getButtons().size(); i++) {
				if (searchPart.getSearchResults().getButtons()[i].isMouseOnButton(event)) {
					if (searchPart.getSearchResults().getButtons()[i].turnHoverOn()) {
						searchResultsDrawing = true;
					}
					if (FROM_LEFT_1ST_BUTTON_PRESSED & event.Event.MouseEvent.dwButtonState) {
						if (i == 0) {
							searchPart.getSearchResults().removeMenuFromScreen(outputHandle);
							itemsDrawing = true;
							activePart = FILES;
						}
						else {
							startOpeningProcess(searchPart.getSearchResults().getButtons()[i].getTextInLine());
							if (!errorDrawing) {
								searchPart.getSearchResults().removeMenuFromScreen(outputHandle);
							}
						}
					}
				}
				else {
					if (searchPart.getSearchResults().getButtons()[i].turnHoverOff()) {
						searchResultsDrawing = true;
					}
				}
			}
		}
	}

	void Program::performOptionsEvents(const INPUT_RECORD &event) {
		activePart = FILES;
		optionsDrawing = false;
		errorDrawing = false;
		mouseClicked = false;
		infoDrawing = false;
		//if (event.Event.MouseEvent.dwButtonState & (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED) && !input.isMouseOnButton(event)) {
		//	isRenameProcess = false;
		//}
		for (int i = 0; i < pointerToOptionsMenu->getButtons().size(); i++) {
			if (pointerToOptionsMenu->getButtons()[i].isMouseOnButton(event)) {
				activePart = OPTIONS;
				if (pointerToOptionsMenu->getButtons()[i].turnHoverOn()) {
					optionsDrawing = true;
				}
				if (event.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
					mouseClicked = true;
					if (i == OPEN) {
						std::string searchedPath = getNewPath(chosenButtons[chosenButtons.size() - 1]);
						startOpeningProcess(searchedPath);
					}
					else if (i == SIZE) {
						std::string searchedPath = getNewPath(chosenButtons[chosenButtons.size() - 1]);
						long double size = 0;
						try {
							if (!fs::is_directory(searchedPath) && searchedPath != "") {
								size = fs::file_size(searchedPath) / 1024;
							}
							else {
								for (auto &p : fs::recursive_directory_iterator(searchedPath)) {
									if (!fs::is_directory(p)) {
										size += fs::file_size(p) / 1024;
									}
								}
							}
							optionsDrawing = false;
							activePart = FILES;
							std::string dimensionName = " kilobytes";
							if (size > 1024) {
								size /= 1024;
								dimensionName = " megabytes";
								if (size > 1024) {
									size /= 1024;
									dimensionName = " gigabytes";
								}
							}
							info.setTextAndColor("Size is: " + std::to_string(size) + dimensionName);
							infoDrawing = true;
						}
						catch (...) {
							error.setTextAndColor("Error while counting size");
							errorDrawing = true;
						}
					}
					else if (i == RENAME) {
						startRenaming();
					}
					else if (i == CUT) {
						activePart = FILES;
						optionsDrawing = false;
						savePathes();
						deleteAfterMovingFile = true;
					}
					else if (i == COPY) {
						activePart = FILES;
						optionsDrawing = false;
						deleteAfterMovingFile = false;
						savePathes();
					}
					else if (i == PASTE) {
						activePart = FILES;
						optionsDrawing = false;
						if (deleteAfterMovingFile) {
							error.setTextAndColor(cantRemoveError);
							for (std::string &oldPath : savedPathesInCutAndCopy) {
								try {
									fs::rename(oldPath, getNewPath(getFileOrFolderName(oldPath)));
								}
								catch (...) {
									error.setTextAndColor(error.getTextInLine() + " \"" + getFileOrFolderName(oldPath) + "\",");
									errorDrawing = true;
								}
							}
							savedPathesInCutAndCopy.clear();
						}
						else {
							error.setTextAndColor(cantCopyError);
							for (std::string &oldPath : savedPathesInCutAndCopy) {
								try {
									fs::copy(oldPath, getNewPath(getFileOrFolderName(oldPath)), fs::copy_options::recursive | fs::copy_options::overwrite_existing);
								}
								catch (...) {
									error.setTextAndColor(error.getTextInLine() + " \"" + getFileOrFolderName(oldPath) + "\",");
									errorDrawing = true;
								}
							}
						}
						openFolder(path);
						std::string errorString = error.getTextInLine();
						errorString.erase(errorString.end() - 1);
						error.setTextAndColor(errorString);
					}
					else if (i == CREATE_FOLDER) {
						optionsDrawing = false;
						itemsDrawing = true;
						try {
							fs::create_directory(path + "/NEW TEMP FOLDER");
						}
						catch (...) {
							error.setTextAndColor(cantCreateFolder);
							errorDrawing = true;
						}
						openFolder(path);
					}
					else if (i == CREATE_FILE) {
						optionsDrawing = false;
						itemsDrawing = true;
						std::ofstream newFile(path + "/NEW CREATED FILE");
						if (newFile) {
							newFile.close();
						}
						else {
							error.setTextAndColor(cantCreateFile);
							errorDrawing = true;
						}
						openFolder(path);
					}
					else if (i == DEL) {
						optionsDrawing = false;
						itemsDrawing = true;
						for (int i = 0; i < chosenButtons.size(); i++) {
							try {
								if (chosenButtons[i]) {
									fs::remove_all(getNewPath(chosenButtons[i]));
								}
							}
							catch (...) {
								error.setTextAndColor(cantDeleteError);
								errorDrawing = true;
							}
						}
						openFolder(path);
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
	}

	void Program::handleFilesPartEventsWhenLeftMouseButtonPressed(ChoosableButton &button, const INPUT_RECORD &event) {
		int index = &button - &items.getButtons()[0];
		if (button.turnChosenStateOn()) {
			chosenButtons.push_back(index);
			itemsDrawing = true;
		}
		else {
			if (CtrlisPressed) {
				button.turnChosenStateOff();
				chosenButtons.erase(std::remove(chosenButtons.begin(), chosenButtons.end(), index), chosenButtons.end());
				itemsDrawing = true;
			}
			else {
				std::string searchedPath = getNewPath(index);
				startOpeningProcess(searchedPath);
			}
		}
	}

	void Program::handleEventsWhenRightMouseButtonPressedOfFilesPart(ChoosableButton &button, const INPUT_RECORD &event) {
		int index = &button - &items.getButtons()[0];
		if (button.turnChosenStateOn()) {
			chosenButtons.push_back(index);
		}
		optionsDrawing = true;
		if (path == "") {
			pointerToOptionsMenu = &diskOptions;
		}
		else {
			pointerToOptionsMenu = &options;
		}
		pointerToOptionsMenu->setStartPosition(COORD{ event.Event.MouseEvent.dwMousePosition.X, event.Event.MouseEvent.dwMousePosition.Y });
	}

	void Program::drawItemsAccordingToStates() {
		if (mouseClicked) {
			info.removeFromConsoleScreen(outputHandle);
			if (!errorDrawing) {
				error.removeFromConsoleScreen(outputHandle);
			}
		}
		if (activePart == SEARCH_RESULTS) {
			if (searchResultsDrawing) {
				pointerToOptionsMenu->removeMenuFromScreen(outputHandle);
				items.removeMenuFromScreen(outputHandle);
				searchPart.getSearchResults().drawMenu(outputHandle);
			}
		}

		else if (activePart == EDIT_FILE) {
			if (textEditDrawing) {
				pointerToOptionsMenu->removeMenuFromScreen(outputHandle);
				items.removeMenuFromScreen(outputHandle);
				fileEditor.drawMenu(outputHandle);
			}
		}
		
		else if (activePart == FILES) {
			pointerToOptionsMenu->removeMenuFromScreen(outputHandle);
			if (itemsDrawing) {
				items.drawMenu(outputHandle);
			}
			if (optionsDrawing) {
				pointerToOptionsMenu->drawMenu(outputHandle);
				activePart = OPTIONS;
			}
			else {
				pointerToOptionsMenu->removeMenuFromScreen(outputHandle);
			}
		}	
		else if (activePart == OPTIONS) {
				if (optionsDrawing) {
					pointerToOptionsMenu->drawMenu(outputHandle);
				}
		}
		if (errorDrawing || error.getIsOnScreenState()) {
			error.appearOnConsoleScreen(outputHandle);
		}
		if (infoDrawing || info.getIsOnScreenState()) {
			info.appearOnConsoleScreen(outputHandle);
		}
	}


	void Program::handleEventsWhenMouseIsNotOnItems(ChoosableButton &button, const INPUT_RECORD &event) {
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
			ar.push_back(itemName);
		}
		return ar;
	}

	void Program::doRenameOperations() {
		if (isRenameProcess) {
			input.appearOnConsoleScreen(outputHandle);
		}
		else {
			if (input.getIsOnScreenState()) {
				input.removeFromConsoleScreen(outputHandle);
				try {
					if (items.getMenuItemStrings()[chosenButtons[chosenButtons.size() - 1]] != "..") {
						std::string searchedPath = getNewPath(chosenButtons[chosenButtons.size() - 1]);
						fs::rename(searchedPath, path + "/" + input.getTextInLine());
						refreshItems();
					}
					else {
						throw;
					}
				}
				catch (...) {
					error.setTextAndColor(cantRenameError);
					errorDrawing = true;
				}

			}
		}
	}

	void Program::savePathes() {
		savedPathesInCutAndCopy.clear();
		for (int index : chosenButtons) {
			if (index != 0) {
				savedPathesInCutAndCopy.push_back(path + "/" + items.getMenuItemStrings()[index]);
			}
		}
	}

	std::string Program::getFileOrFolderName(std::string &oldPath) {
		size_t pos = oldPath.find_last_of('/');
		std::string name = oldPath.substr(pos + 1);
		return name;
	}

	void Program::refreshItems() {
		items.removeMenuFromScreen(outputHandle);
		items.setMenuItems(getContentOfFolder(path));
		itemsDrawing = true;
		drawItemsAccordingToStates();
	}

	void Program::scrollVertically(const INPUT_RECORD &event) {
		CONSOLE_SCREEN_BUFFER_INFO screenInfo;
		SMALL_RECT win;
		
		if (!GetConsoleScreenBufferInfo(outputHandle, &screenInfo)) {
			return;
		}
		win = screenInfo.srWindow;

		int delta = static_cast<short> HIWORD(event.Event.MouseEvent.dwButtonState) / 100;
		win.Top -= delta;
		win.Bottom -= delta;

		SetConsoleWindowInfo(outputHandle, TRUE, &win);
	}

	void Program::scrollHorizontally(const INPUT_RECORD &event) {
		CONSOLE_SCREEN_BUFFER_INFO screenInfo;
		SMALL_RECT win;

		if (!GetConsoleScreenBufferInfo(outputHandle, &screenInfo)) {
			return;
		}
		win = screenInfo.srWindow;

		int delta = static_cast<short> HIWORD(event.Event.MouseEvent.dwButtonState) / 100;
		win.Left += delta;
		win.Right += delta;

		SetConsoleWindowInfo(outputHandle, TRUE, &win);
	}

	void Program::startRenaming() {
		activePart = FILES;
		optionsDrawing = false;
		isRenameProcess = true;
		pointerToOptionsMenu->removeMenuFromScreen(outputHandle);
		input.setTextAndColor(items.getMenuItemStrings()[chosenButtons[chosenButtons.size() - 1]]);
		input.setMinLength(items.getButtons()[chosenButtons[chosenButtons.size() - 1]].getMinLength());
		input.setPosition(items.getButtons()[chosenButtons[chosenButtons.size() - 1]].getStartPosition());
		input.turnInputStateOn();
		doRenameOperations();
	}

	void Program::performTextEditingEvents(const INPUT_RECORD &event) {
		textEditDrawing = false;
		if (FROM_LEFT_1ST_BUTTON_PRESSED & event.Event.MouseEvent.dwButtonState) {
			for (int i = 0; i < fileEditor.getButtons().size(); i++) {
				if (fileEditor.getButtons()[i].isMouseOnButton(event)) {
					fileEditor.getButtons()[i].turnInputStateOn(event);
					textEditDrawing = true;
					fileEditor.setInputLineIndex(i);
				}
				else {
					if (fileEditor.getButtons()[i].turnInputStateOff()) {
						textEditDrawing = true;
					}
				}
			}
		}
	}


