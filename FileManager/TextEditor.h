#ifndef TxtEdt
#define TxtEdt

#include "TemplateMenu.h"
#include "InputForm.h"


class TextEditor : public TemplateMenu<InputForm> {
public:
	TextEditor();
	TextEditor(std::vector<std::string> itemsVector);

};




#endif
