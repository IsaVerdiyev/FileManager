#ifndef TxtEdt
#define TxtEdt

#include "TemplateMenu.h"
#include "InputForm.h"


class TextEditor : public TemplateMenu<InputForm> {
	int inputLineIndex;
	Color activeColor;
	Color deactiveColor;
	Color cursorColor;
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
};




#endif
