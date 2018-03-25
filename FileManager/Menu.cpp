#include "Menu.h"

void Menu::setMenuItems(const std::vector<std::string> &items) {
	setMenuItemStrings(items);
	setMenuItemButtons();
}

void Menu::setMenuItemStrings(const std::vector<std::string> &items) {
	menuItemStrings = items;
}

void Menu::setMenuItemButtons() {
	int maxLength = getLengthOfItems();
	menuItemButtons.clear();
	for (int i = 0; i < menuItemStrings.size(); i++) {
		menuItemButtons.push_back(ChoosableButton(menuItemStrings[i]));
		menuItemButtons[i].setPosition({ startPositionOfDisplay.X, (short)(startPositionOfDisplay.Y + i) });
		menuItemButtons[i].setMinLength(maxLength);
	}
}

int Menu::getLengthOfItems() {
	int maxLength = 0;
	for (std::string str : menuItemStrings) {
		if (maxLength < str.size() + 1) {
			maxLength = str.size() + 1;
		}
	}
	return maxLength;
}

void Menu::drawMenu(HANDLE &hndl) {
	for (ChoosableButton &button : menuItemButtons) {
		button.appearOnConsole(hndl);
	}
}

void Menu::setStartPosition(COORD pos) {
	startPositionOfDisplay = pos;
	for (int i = 0; i < menuItemButtons.size(); i++) {
		menuItemButtons[i].setPosition({ pos.X, (short)(pos.Y + i)});
	}
}