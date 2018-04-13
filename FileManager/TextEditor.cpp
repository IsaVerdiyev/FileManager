#include "TextEditor.h"

TextEditor::TextEditor() : TemplateMenu() {}

TextEditor::TextEditor(const std::vector<std::string> &itemsVector) :
	TemplateMenu(itemsVector) {
	setColors();
}

void TextEditor::setInputLineIndex(int index) {
	inputLineIndex = index;
}

int TextEditor::getInputLineIndex() {
	return inputLineIndex;
}

void TextEditor::setActiveColor(Color c) {
	activeColor = c;
	for (InputForm &inputLine : menuItemButtons) {
		inputLine.setActiveColor(c);
	}
}

void TextEditor::setDeactiveColor(Color c) {
	deactiveColor = c;
	for (InputForm &inputLine : menuItemButtons) {
		inputLine.setDeactiveColor(c);
	}
}

void TextEditor::setCursorColor(Color c){
	cursorColor = c;
	for (InputForm &inputLine : menuItemButtons) {
		inputLine.setInputIndexColor(c);
	}
}

void TextEditor::setColors() {
	for (InputForm &inputLine : menuItemButtons) {
		inputLine.setActiveColor(activeColor);
		inputLine.setDeactiveColor(deactiveColor);
		inputLine.setInputIndexColor(cursorColor);
	}
}

void TextEditor::setMenuItems(const std::vector<std::string> &itemsVector) {
	TemplateMenu::setMenuItems(itemsVector);
	setColors();
}
