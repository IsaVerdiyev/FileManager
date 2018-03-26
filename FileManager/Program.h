#ifndef Prog
#define Prog

#include "Menu.h"

class Program {
	enum menuPart {FILES, OPTIONS};
	std::vector<std::string> disks;
	bool CTRLisPressed;
	bool activePart;
	HANDLE outputHandle;
	HANDLE inputHandle;
	Menu options;
	Menu items;
	std::string path;
	INPUT_RECORD eventsBuffer[128];
public:
	Program();
	void setDisks();
	void performCycleOfEvents();
	void checkMouseEvent(INPUT_RECORD &event);
	void checkKeyEvent(INPUT_RECORD &event);
};

#endif // Prog