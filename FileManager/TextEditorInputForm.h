#ifndef TxtEdtInpFrm
#define TxtEdtInpFrm

/*
	This class is inherited from InputForm and is used in TextEditor class.
	It is different from InputForm in  function : void takeInput(const INPUT_RECORD &event).
	Here when trying to insert to end of object  no inserting appears.
	In InputForm class it is possible to insert at the end of object.
	It is impossible to put cursor on position where there is no symbol added by user
	Also there is difference in setting stringSize when is called takeInput function
*/

#include "InputForm.h"

class TextEditorInputForm : public InputForm {
public:
	TextEditorInputForm();
	TextEditorInputForm(const std::string &s);
	void takeInput(const INPUT_RECORD &event);
	virtual void setCursorPositionIndex(int index) override;
	void setVisibleStringSize(int s);
	std::string getVisibleString();
	size_t findIndexInTextLine(int visibleIndex);
};














#endif // TxtEdtInpFrm
