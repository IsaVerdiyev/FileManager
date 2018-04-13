#include "TextEditor.h"

TextEditor::TextEditor() : TemplateMenu() {}

TextEditor::TextEditor(std::vector<std::string> itemsVector) :
	TemplateMenu(itemsVector) {}

void TextEditor::setInputLineIndex(int index) {
	inputLineIndex = index;
}

int TextEditor::getInputLineIndex() {
	return inputLineIndex;
}

