#ifndef TempMen
#define TempMen

#include <vector>
#include <Windows.h>
#include <string>
#include "Colors.h"
#include "HelperFunctions.h"

template<class T>
class TemplateMenu {
protected:
	bool isOnScreen = false;
	bool lengthChanged = false;
	bool positionChanged = false;
	bool amountOfLinesChanged = false;
	int lengthOfItems = 0;
	std::vector<std::string> menuItemStrings;
	std::vector<T> menuItemButtons;
	std::vector<T> eraseItemButtons;
	COORD startPositionOfDisplay;

	void setMenuItemStrings(const std::vector<std::string> &items) {
		menuItemStrings = items;
	}

	void setMenuItemButtons() {
		setLengthOfItems();
		menuItemButtons.clear();
		for (int i = 0; i < menuItemStrings.size(); i++) {
			menuItemButtons.push_back(T(menuItemStrings[i]));
			menuItemButtons[i].setPosition({ startPositionOfDisplay.X, static_cast<short>(startPositionOfDisplay.Y + i) });
			menuItemButtons[i].setMinLength(lengthOfItems);
		}
	}

	void setLengthOfItems() {
		if (lengthOfItems != searchLengthOfItems()) {
			lengthChanged = true;
			lengthOfItems = searchLengthOfItems();
		}
	}

	int searchLengthOfItems() {
		int maxLengthOfString = 0;
		for (std::string &str : menuItemStrings) {
			std::string visibleStr = HelperFunctions::getStringWithReplacedSlashT_ToSpaces(str, TextLine::slashT_SpaceCounts);
			if (maxLengthOfString < visibleStr.size() + 1) {
				maxLengthOfString = visibleStr.size() + 1;
			}
		}
		return maxLengthOfString;
	}


public:
	TemplateMenu() {};

	TemplateMenu(const std::vector<std::string> &itemsVector) {
		setMenuItems(itemsVector);
	}

	void setStartPosition(COORD pos) {
		startPositionOfDisplay = pos;
		positionChanged = true;
		for (int i = 0; i < menuItemButtons.size(); i++) {
			menuItemButtons[i].setPosition({ pos.X, static_cast<short>(pos.Y + i) });
		}
	}

	void setMenuItems(const std::vector<std::string> &items) {
		setMenuItemStrings(items);
		if (items.size() != menuItemButtons.size()) {
			amountOfLinesChanged = true;
		}
		setMenuItemButtons();
		
	}

	void removeMenuFromScreen(HANDLE &hndl) {
		if (isOnScreen) {
			for (T &button : menuItemButtons) {
				button.removeFromConsoleScreen(hndl);
			}
			isOnScreen = false;
		}
	}

	void drawMenu(HANDLE &hndl) {
		if (!isOnScreen) {
			for (T &button : menuItemButtons) {
				button.appearOnConsoleScreen(hndl);
			}
			eraseItemButtons = menuItemButtons;
			lengthChanged = false;
			positionChanged = false;
			amountOfLinesChanged = false;
		}
		else if (isOnScreen && (lengthChanged || positionChanged || amountOfLinesChanged)) {
			for (T &button : eraseItemButtons) {
				button.removeFromConsoleScreen(hndl);
			}
			lengthChanged = false;
			positionChanged = false;
			amountOfLinesChanged = false;
			for (T &button : menuItemButtons) {
				button.appearOnConsoleScreen(hndl);
			}
			eraseItemButtons = menuItemButtons;
		}
		else {
			for (T &button : menuItemButtons) {
				button.appearOnConsoleScreen(hndl);
			}
		}
		isOnScreen = true;
	}

	std::vector<T> &getButtons() {
		return menuItemButtons;
	}

	std::vector<std::string> &getMenuItemStrings() {
		return menuItemStrings;
	}

	bool isLengthChanged() {
		return lengthOfItems != searchLengthOfItems();
	}
};


#endif // TempMen
