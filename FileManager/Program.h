#ifndef Prog
#define Prog

#include "Menu.h"
#include <io.h>;
#include "HelperFunctions.h"

class Program {
	enum menuPart {FILES, OPTIONS};
	std::vector<std::string> disks;
	bool CTRLisPressed;
	menuPart activePart;
	HANDLE outputHandle;
	HANDLE inputHandle;
	Menu options;
	Menu items;
	Menu diskOptions;
	std::string path;
	INPUT_RECORD eventsBuffer[128];
public:
	Program();
	void setDisks();
	void performCycleOfEvents();
	void checkMouseEvent(INPUT_RECORD &event);
	void checkKeyEvent(INPUT_RECORD &event);
	void setNewPath(int index);
	void openFolder(int index);
	std::vector<std::string> getNewItemStringsFromNewPath();
	std::vector<std::string> getFiles(std::string path);
};

#endif // Prog