#ifndef TxtEdt
#define TxtEdt

#include "TemplateMenu.h"
#include "InputForm.h"


class TextEditor : public TemplateMenu<InputForm> {
	int inputLineIndex;
public:
	TextEditor();
	TextEditor(std::vector<std::string> itemsVector);
	void setInputLineIndex(int index);
	int getInputLineIndex();
};




#endif
