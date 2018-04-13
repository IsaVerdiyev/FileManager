#include "Menu.h"


Menu::Menu(): TemplateMenu() {}

Menu::Menu(const std::vector<std::string> &itemsVector): 
	TemplateMenu(itemsVector) {
	setColorOfItems();
}

void Menu::setHoverColor(Color c) {
	hoverColor = c;
	for (ChoosableButton &button : menuItemButtons) {

		button.setHovercolor(hoverColor);
	}
}

void Menu::setButtonColor(Color c) {
	buttonColor = c;
	for (ChoosableButton &button : menuItemButtons) {
		button.setButtonColor(buttonColor);
	}
}


void Menu::setChosenColor(Color c) {
	chosenColor = c;
	for (ChoosableButton &button : menuItemButtons) {
		button.setChosenButtonColor(chosenColor);
	}
}
void Menu::setHoverOnChosenColor(Color c) {
	hoverOnChosenColor = c;
	for (ChoosableButton &button : menuItemButtons) {
		button.setHoverOnChosenColor(hoverOnChosenColor);
	}
}

void Menu::setColorOfItems() {
	for (ChoosableButton &button : menuItemButtons) {
		button.setHovercolor(hoverColor);
		button.setButtonColor(buttonColor);
		button.setChosenButtonColor(chosenColor);
		button.setHoverOnChosenColor(hoverOnChosenColor);
	}
}

void Menu::setMenuItems(const std::vector<std::string> &itemsVector) {
	TemplateMenu::setMenuItems(itemsVector);
	setColorOfItems();
}