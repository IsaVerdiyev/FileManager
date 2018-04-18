#ifndef TxtEdtInpFrm
#define TxtEdtInpFrm

/*
	This class is inherited from InputForm and is used in TextEditor class.
	It is different from InputForm in only one function : void takeInput(const INPUT_RECORD &event).
	Here when trying to insert to end of object  no inserting appears.
	In InputForm class it is possible to insert at the end of object.
*/

#include "InputForm.h"

class TextEditorInputForm : public InputForm {
public:
	TextEditorInputForm();
	TextEditorInputForm(const std::string &s);
	void takeInput(const INPUT_RECORD &event);
};














#endif // TxtEdtInpFrm
