#ifndef MN
#define MN

#include "TemplateMenu.h"
#include "ChoosableButton.h"

class Menu : public TemplateMenu<ChoosableButton> {
public:
	Menu();
	Menu(std::vector<std::string> itemsVector);
	void setHoverColor(Color c);
	void setStandardColor(Color c);
};











#endif //MN
