#ifndef TempMen
#define TempMen

#include <vector>
#include <Windows.h>
#include <string>
#include "Colors.h"

template<class T>
class TemplateMenu {
	
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
			menuItemButtons[i].setPosition({ startPositionOfDisplay.X, (short)(startPositionOfDisplay.Y + i) });
			menuItemButtons[i].setMinLength(lengthOfItems);
		}
	}

	void setLengthOfItems() {
		lengthOfItems = 0;
		for (std::string str : menuItemStrings) {
			if (lengthOfItems < str.size() + 1) {
				lengthOfItems = str.size() + 1;
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
			menuItemButtons[i].setPosition({ pos.X, (short)(pos.Y + i) });
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

	void setHoverColor(Color c) {
		for (T &button : menuItemButtons) {
			button.setHovercolor(c);
		}
	}

	void setStandardColor(Color c) {
		for (T &button : menuItemButtons) {
			button.setButtonColor(c);
		}
	}
};


#endif // TempMen
