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
	int lengthOfItems;
	std::vector<std::string> menuItemStrings;
	std::vector<T> menuItemButtons;
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
		lengthOfItems = 0;
		for (std::string &str : menuItemStrings) {
			std::string visibleStr = HelperFunctions::getStringWithReplacedSlashT_ToSpaces(str, TextLine::slashT_SpaceCounts);
			if (lengthOfItems < visibleStr.size() + 1) {
				lengthOfItems = visibleStr.size() + 1;
			}
		}
	}

public:
	TemplateMenu() {};

	TemplateMenu(std::vector<std::string> itemsVector) {
		setMenuItems(itemsVector);
	}

	void setStartPosition(COORD pos) {
		startPositionOfDisplay = pos;
		for (int i = 0; i < menuItemButtons.size(); i++) {
			menuItemButtons[i].setPosition({ pos.X, static_cast<short>(pos.Y + i) });
		}
	}

	void setMenuItems(const std::vector<std::string> &items) {
		setMenuItemStrings(items);
		setMenuItemButtons();
	}

	void removeMenuFromScreen(HANDLE &hndl) {
		for (T &button : menuItemButtons) {
			button.removeFromConsoleScreen(hndl);
		}
	}

	void drawMenu(HANDLE &hndl) {
		for (T &button : menuItemButtons) {
			button.appearOnConsoleScreen(hndl);
		}
	}

	std::vector<T> &getButtons() {
		return menuItemButtons;
	}

	std::vector<std::string> &getMenuItemStrings() {
		return menuItemStrings;
	}

};


#endif // TempMen
