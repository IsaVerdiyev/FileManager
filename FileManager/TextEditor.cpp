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
	setMenuItemStrings(itemsVector);
	if (itemsVector.size() != menuItemButtons.size()) {
		amountOfLinesChanged = true;
	}
	setMenuItemButtons();
	setColors();
}

void TextEditor::setMenuItemButtons() {
	menuItemButtons.clear();
	for (int i = 0; i < menuItemStrings.size(); i++) {
		menuItemButtons.push_back(TextEditorInputForm(menuItemStrings[i]));
		menuItemButtons[i].setPosition({ startPositionOfDisplay.X, static_cast<short>(startPositionOfDisplay.Y + i) });
		menuItemButtons[i].setMinLength(lengthOfLineInTextEditor);
	}
}

void TextEditor::setLengthOfLineInTextEditor(int l) {
	lengthOfLineInTextEditor = l;
}

void TextEditor::addNewLine(HANDLE hndl) {
	std::string removedPart;
	int index = menuItemButtons[inputLineIndex].findIndexInTextLine(menuItemButtons[inputLineIndex].getCursorIndexPosition());
	for (int i = index; i < menuItemStrings[inputLineIndex].size(); i++) {
		removedPart.push_back(menuItemStrings[inputLineIndex][i]);
	}
	menuItemStrings[inputLineIndex].erase(menuItemStrings[inputLineIndex].begin() + index, menuItemStrings[inputLineIndex].end());
	menuItemStrings.insert(menuItemStrings.begin() + ++inputLineIndex, removedPart);
	setMenuItems(menuItemStrings);
	menuItemButtons[inputLineIndex].turnInputStateOn();
	menuItemButtons[inputLineIndex].setCursorPositionIndex(0);

	//for (int i = menuItemButtons[inputLineIndex].getCursorIndexPosition(); i < menuItemButtons[inputLineIndex].getVisibleStringSize(); i++) {
	//	
	//	//removedPart.push_back(menuItemButtons[inputLineIndex].getSentenceSymbols()[i].Char.AsciiChar);
	//}
	
	//getMenuItemStrings().insert(getMenuItemStrings().begin() + inputLineIndex + 1, removedPart);
	///*if (isLengthChanged()) {
	//	TemplateMenu::removeMenuFromScreen(hndl);
	//}*/
	//setMenuItems(getMenuItemStrings());
	//inputLineIndex++;
	//getButtons()[inputLineIndex].turnInputStateOn();
	//getButtons()[inputLineIndex].setCursorPositionIndex(0);
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

void TextEditor::removeLine(HANDLE hndl) {
	int futureCursorPosition = menuItemButtons[inputLineIndex - 1].getVisibleStringSize();
	menuItemStrings[inputLineIndex - 1] += menuItemStrings[inputLineIndex];
	menuItemStrings.erase(menuItemStrings.begin() + inputLineIndex);
	/*if (isLengthChanged()) {
		TemplateMenu::removeMenuFromScreen(hndl);
	}*/
	setMenuItems(getMenuItemStrings());
	inputLineIndex--;
	menuItemButtons[inputLineIndex].turnInputStateOn();
	menuItemButtons[inputLineIndex].setCursorPositionIndex(futureCursorPosition);
}

void TextEditor::setPathToFile(const std::string &path) {
	pathToFile = path;
}

void TextEditor::saveFile() {
	std::ofstream file(pathToFile);
	if (!file) {
		throw std::runtime_error("Coudn't save file");
	}
	HelperFunctions::setTextInNewFile(file, menuItemStrings);
}
