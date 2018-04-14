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
		inputLine.setCursorColor(c);
	}
}

void TextEditor::setColors() {
	for (InputForm &inputLine : menuItemButtons) {
		inputLine.setActiveColor(activeColor);
		inputLine.setDeactiveColor(deactiveColor);
		inputLine.setCursorColor(cursorColor);
	}
}

void TextEditor::setMenuItems(const std::vector<std::string> &itemsVector) {
	TemplateMenu::setMenuItems(itemsVector);
	setColors();
}

void TextEditor::addNewLine() {
	std::string removedPart;
	for (int i = menuItemButtons[inputLineIndex].getCursorIndexPosition(); i < menuItemButtons[inputLineIndex].getVisibleStringSize(); i++) {
		removedPart.push_back(menuItemButtons[inputLineIndex].getSentenceSymbols()[i].Char.AsciiChar);
	}
	getMenuItemStrings().insert(getMenuItemStrings().begin() + inputLineIndex + 1, removedPart);
	setMenuItems(getMenuItemStrings());
	inputLineIndex++;
	getButtons()[inputLineIndex].turnInputStateOn();
	/*INPUT_RECORD event;
	event.Event.KeyEvent.wVirtualKeyCode = VK_BACK;
	while (menuItemButtons[inputLineIndex].getCursorIndexPosition() < menuItemButtons[inputLineIndex].getVisibleStringSize()) {
		menuItemButtons[inputLineIndex].setCursorPositionIndex(menuItemButtons[inputLineIndex].getCursorIndexPosition() + 1);
		menuItemButtons[inputLineIndex].takeInput(event);
	}
	menuItemButtons[inputLineIndex].turnInputStateOff();
	menuItemStrings[inputLineIndex] = menuItemButtons[inputLineIndex].getTextInLine();
	inputLineIndex++;
	menuItemStrings.insert(menuItemStrings.begin() + inputLineIndex, removedPart);
	setMenuItems(getMenuItemStrings());
	menuItemButtons[inputLineIndex].turnInputStateOn();
	*/
	//==============
	/*menuItemButtons[inputLineIndex].turnInputStateOff();
	menuItemButtons.insert(menuItemButtons.begin() + inputLineIndex + 1, InputForm(removedPart));

	menuItemStrings.insert(menuItemStrings.begin() + inputLineIndex + 1, removedPart);
	inputLineIndex++;
	menuItemButtons[inputLineIndex].setMinLength(menuItemButtons[0].getSentenceSymbols().size());
	menuItemButtons[inputLineIndex].setDeactiveColor(deactiveColor);
	menuItemButtons[inputLineIndex].setActiveColor(activeColor);
	menuItemButtons[inputLineIndex].setCursorColor(cursorColor);
	menuItemButtons[inputLineIndex].turnInputStateOn();*/
}

void TextEditor::removeLine() {
	menuItemStrings[inputLineIndex - 1] += menuItemStrings[inputLineIndex];
	menuItemStrings.erase(menuItemStrings.begin() + inputLineIndex);
	setMenuItems(getMenuItemStrings());
	inputLineIndex--;
	menuItemButtons[inputLineIndex].turnInputStateOn();
}