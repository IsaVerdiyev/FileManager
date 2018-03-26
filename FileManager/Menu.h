#ifndef MN
#define MN

#include "ChoosableButton.h"

class Menu {
	bool isActive = true;
	int lengthOfItems;
	std::vector<CHAR_INFO> sentenceSymbols;
	std::vector<CHAR_INFO> eraseArray;
	std::vector<std::string> menuItemStrings;
	std::vector<ChoosableButton> menuItemButtons;
	COORD startPositionOfDisplay;
	void setMenuItemStrings(const std::vector<std::string> &items);
	void setMenuItemButtons();
	void setLengthOfItems();
public:
	Menu();
	Menu(std::vector<std::string> itemsVector);
	void setStartPosition(COORD pos);
	void setMenuItems(const std::vector<std::string> &items);
	void setEraseArray();
	void removeMenuFromScreen(HANDLE &hndl);
	void drawMenu(HANDLE &hndl);
	std::vector<ChoosableButton> &getButtons();
};


#endif // MN
