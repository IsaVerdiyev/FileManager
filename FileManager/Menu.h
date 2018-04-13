#ifndef MN
#define MN

#include "TemplateMenu.h"
#include "ChoosableButton.h"

class Menu : public TemplateMenu<ChoosableButton> {
	Color hoverColor = defaultHoverColor;
	Color buttonColor = defaultButtonColor;
	Color chosenColor = defaultChosenButtonColor;
	Color hoverOnChosenColor = defaultHoverOnChosenColor;
public:
	Menu();
	Menu(const std::vector<std::string> &itemsVector);
	void setMenuItems(const std::vector<std::string> &itemsVector);
	void setHoverColor(Color c);
	void setButtonColor(Color c);
	void setChosenColor(Color c);
	void setColorOfItems();
	void setHoverOnChosenColor(Color c);
};











#endif //MN
