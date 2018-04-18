#ifndef TxtEdt
#define TxtEdt

#include "TemplateMenu.h"
#include "TextEditorInputForm.h"


class TextEditor : public TemplateMenu<TextEditorInputForm> {
	int inputLineIndex;
	Color activeColor;
	Color deactiveColor;
	Color cursorColor;
	std::string pathToFile;
	int lengthOfLineInTextEditor = 0;
public:
	TextEditor();
	TextEditor(const std::vector<std::string> &itemsVector);
	void setMenuItems(const std::vector<std::string> &itemsVector);
	void setInputLineIndex(int index);
	int getInputLineIndex();
	void setActiveColor(Color c);
	void setDeactiveColor(Color c);
	void setCursorColor(Color c);
	void setColors();
	void addNewLine(HANDLE hndl);
	void removeLine(HANDLE hndl);
	void setPathToFile(const std::string &path);
	void saveFile();
	void setMenuItemButtons();
	void setLengthOfLineInTextEditor(int l);
};




#endif
