#ifndef MN
#define MN

/*
	Class Menu is inherited from TemplateMenu class with ChoosableButton instead of T.
	This class is created for operating with array of ChoosableButton objects.
*/

#include "TemplateMenu.h"
#include "ChoosableButton.h"

class Menu : public TemplateMenu<ChoosableButton> {
	Color hoverColor = defaultHoverColor; //These variables
	Color buttonColor = defaultButtonColor;// are created for putting
	Color chosenColor = defaultChosenButtonColor;// colors of objects in specified
	Color hoverOnChosenColor = defaultHoverOnChosenColor;// in these variables colors
public:
	// Constructors call TemplateMenu constructors and additionally 
	// put colors of objects in colors specified in variables of this class
	Menu(); 
	Menu(const std::vector<std::string> &itemsVector);
	//////////////////////////////////////////////////////


	void setMenuItems(const std::vector<std::string> &itemsVector);
	// calls same function in TemplateMenu class and additionally sets colors of objects

	// these functions just set colors that future objects will have
	void setHoverColor(Color c);
	void setButtonColor(Color c);
	void setChosenColor(Color c);
	void setHoverOnChosenColor(Color c);

	// function sets color of objects
	void setColorOfItems();
};











#endif //MN
