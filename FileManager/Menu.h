#ifndef MN
#define MN

#include "ChoosableButton.h"

class Menu {
	bool isActive = true;
	std::vector<std::string> menuItemStrings;
	std::vector<ChoosableButton> menuItemButtons;
	COORD startPositionOfDisplay;
	void setMenuItemStrings(const std::vector<std::string> &items);
	void setMenuItemButtons();
	int getLengthOfItems();
public:
	void setStartPosition(COORD pos);
	void setMenuItems(const std::vector<std::string> &items);
	void drawMenu(HANDLE &hndl);
};


#endif // MN
