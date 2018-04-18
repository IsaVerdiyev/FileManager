#ifndef TxtEdtInpFrm
#define TxtEdtInpFrm

#include "InputForm.h"

class TextEditorInputForm : public InputForm {
public:
	TextEditorInputForm();
	TextEditorInputForm(const std::string &s);
	void takeInput(const INPUT_RECORD &event);
};














#endif // TxtEdtInpFrm
