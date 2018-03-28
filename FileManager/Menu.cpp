#include "Menu.h"

Menu::Menu() {}

Menu::Menu(std::vector<std::string> itemsVector) {
	setMenuItems(itemsVector);
}

void Menu::setMenuItems(const std::vector<std::string> &items) {
	setMenuItemStrings(items);
	setMenuItemButtons();
}

void Menu::setMenuItemStrings(const std::vector<std::string> &items) {
	menuItemStrings = items;
}

void Menu::setMenuItemButtons() {
	setLengthOfItems();
	menuItemButtons.clear();
	for (int i = 0; i < menuItemStrings.size(); i++) {
		menuItemButtons.push_back(ChoosableButton(menuItemStrings[i]));
		menuItemButtons[i].setPosition({ startPositionOfDisplay.X, (short)(startPositionOfDisplay.Y + i) });
		menuItemButtons[i].setMinLength(lengthOfItems);
	}
}

void Menu::setLengthOfItems() {
	lengthOfItems = 0;
	for (std::string str : menuItemStrings) {
		if (lengthOfItems < str.size() + 1) {
			lengthOfItems = str.size() + 1;
		}
	}
}



void Menu::removeMenuFromScreen(HANDLE &hndl) {
	for (ChoosableButton &button : menuItemButtons) {
		button.removeFromConsoleScreen(hndl);
	}
}

void Menu::drawMenu(HANDLE &hndl) {
	for (ChoosableButton &button : menuItemButtons) {
		button.appearOnConsoleScreen(hndl);
	}
}

void Menu::setStartPosition(COORD pos) {
	startPositionOfDisplay = pos;
	for (int i = 0; i < menuItemButtons.size(); i++) {
		menuItemButtons[i].setPosition({ pos.X, (short)(pos.Y + i)});
	}
}

std::vector<ChoosableButton> &Menu::getButtons() {
	return menuItemButtons;
}

std::vector<std::string> &Menu::getMenuItemStrings() {
	return menuItemStrings;
}

void Menu::setHoverColor(Color c) {
	for (ChoosableButton &button : menuItemButtons) {
		button.setHovercolor(c);
	}
}
void Menu::setStandardColor(Color c) {
	for (ChoosableButton &button : menuItemButtons) {
		button.setButtonColor(c);
	}
}