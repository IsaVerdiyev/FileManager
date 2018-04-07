#ifndef SeaTab

#include "Menu.h"
#include "InputForm.h"
#include <string>


class SearchTable {
	COORD startPosition;
	TextLine searchHeader;
	InputForm searchInput;
public:
	SearchTable();
	void setStartPosition(COORD start);
	void setMinLength();
	void appearOnConsole(HANDLE &hndl);
	bool isMouseOnSearch(const INPUT_RECORD &event);
	void takeInput(const INPUT_RECORD &event);
	bool isGettingInput();
	void setActiveColor(Color c);
	void setDeactiveColor(Color c);
	bool turnInputStateOn();
	bool turnInputStateOff();
};





#endif //SeaTab
