#ifndef Prog
#define Prog

#include "Menu.h"
#include <io.h>;
#include "HelperFunctions.h"
#include <algorithm>
#include <experimental\filesystem>

namespace sf = std::experimental::filesystem;

class Program {
	TextLine error;
	enum menuPart {FILES, OPTIONS};
	std::vector<std::string> disks;
	std::vector<int> chosenButtons;
	bool CtrlisPressed;
	menuPart activePart;
	HANDLE outputHandle;
	HANDLE inputHandle;
	Menu options;
	Menu items;
	Menu diskOptions;
	std::string path;
	INPUT_RECORD eventsBuffer[128];
	enum optionsEnum { OPEN, RENAME, CUT, COPY, PASTE, SIZE, CREATE_FILE, CREATE_FOLDER, DEL };
	enum diskOptionsEnum { OPEN_DISK, SIZE_DISK };
	bool itemsDrawing;
	bool optionsDrawing;
	bool errorDrawing;
	bool mouseClicked;
	bool drawOptions;
public:
	Program();
	void setDisks();
	void performCycleOfEvents();
	void checkMouseEvent(INPUT_RECORD &event);
	void checkKeyEvent(INPUT_RECORD &event);
	std::string getNewPath(int index);
	void openFolder(int index);
	std::vector<std::string> getNewItemStringsFromNewPath();
	std::vector<std::string> getFiles(std::string path);
	void performFilesPartEvents(INPUT_RECORD &event);
	void performOptionsEvents(INPUT_RECORD &event);
	void handleFilesPartEventsWhenLeftMouseButtonPressed(ChoosableButton &button, INPUT_RECORD &event);
	void handleEventsWhenRightMouseButtonPressedOfFilesPart(ChoosableButton &button, INPUT_RECORD &event);
	void drawItemsAccordingToStates();
	void handleEventsWhenMouseIsNotOnItems(ChoosableButton &button, INPUT_RECORD &event);
};

#endif // Prog