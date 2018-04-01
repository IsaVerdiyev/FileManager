#ifndef Prog
#define Prog

#include "Menu.h"
#include <io.h>;
#include "HelperFunctions.h"
#include <algorithm>
#include <experimental\filesystem>
#include "ErrorStrings.h";
#include "InputForm.h"

namespace fs = std::experimental::filesystem;

class Program {
	TextLine error;
	TextLine info;
	InputForm input;
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
	Menu *pointerToOptionsMenu;
	enum optionsEnum { OPEN, RENAME, CUT, COPY, PASTE, SIZE, CREATE_FILE, CREATE_FOLDER, DEL };
	enum diskOptionsEnum { OPEN_DISK, SIZE_DISK };
	bool itemsDrawing;
	bool optionsDrawing;
	bool errorDrawing;
	bool mouseClicked;
	bool infoDrawing;
	bool isRenameProcess = false;
	bool deleteAfterMovingFile = false;
	std::vector<std::string> savedPathes;
public:
	Program();
	void setDisks();
	void performCycleOfEvents();
	void checkMouseEvent(INPUT_RECORD &event);
	void checkKeyEvent(INPUT_RECORD &event);
	std::string getNewPath(int index);
	void openFolder(int index);
	void performFilesPartEvents(INPUT_RECORD &event);
	void performOptionsEvents(INPUT_RECORD &event);
	void handleFilesPartEventsWhenLeftMouseButtonPressed(ChoosableButton &button, INPUT_RECORD &event);
	void handleEventsWhenRightMouseButtonPressedOfFilesPart(ChoosableButton &button, INPUT_RECORD &event);
	void drawItemsAccordingToStates();
	void handleEventsWhenMouseIsNotOnItems(ChoosableButton &button, INPUT_RECORD &event);
	std::vector<std::string> getContentOfFolder(std::string path);
	void doRenameOperations();
	void bufferPathes();
};

#endif // Prog