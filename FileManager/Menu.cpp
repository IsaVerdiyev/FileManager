#include "Menu.h"


Menu::Menu(): TemplateMenu() {}

Menu::Menu(std::vector<std::string> itemsVector): TemplateMenu(itemsVector) {}

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