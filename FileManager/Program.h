#ifndef Prog
#define Prog

#include "TemplateMenu.h"
#include <io.h>;
#include "HelperFunctions.h"
#include <algorithm>
#include <experimental\filesystem>
#include "ErrorStrings.h";
#include "InputForm.h"
#include <fstream>
#include "SearchTable.h"

namespace fs = std::experimental::filesystem;

class Program {
	TextLine error;
	TextLine info;
	InputForm input;
	enum menuPart {FILES, OPTIONS, SEARCH_RESULTS};
	std::vector<std::string> disks;
	std::vector<int> chosenButtons;
	bool CtrlisPressed;
	menuPart activePart;
	HANDLE outputHandle;
	HANDLE inputHandle;
	Menu<ChoosableButton> options;
	Menu<ChoosableButton> items;
	SearchTable searchPart;
	Menu<ChoosableButton> diskOptions;
	std::string path;
	INPUT_RECORD eventsBuffer[128];
	Menu<ChoosableButton> *pointerToOptionsMenu;
	enum optionsEnum { OPEN, RENAME, CUT, COPY, PASTE, SIZE, CREATE_FILE, CREATE_FOLDER, DEL };
	enum diskOptionsEnum { OPEN_DISK, SIZE_DISK };
	bool itemsDrawing;
	bool optionsDrawing;
	bool errorDrawing;
	bool mouseClicked;
	bool infoDrawing;
	bool searchResultsDrawing;
	bool isRenameProcess = false;
	bool isSearchProcess = false;
	bool deleteAfterMovingFile = false;
	std::vector<std::string> savedPathesInCutAndCopy;
public:
	Program();
	void setDisks();
	void performCycleOfEvents();
	void checkMouseEvent(INPUT_RECORD &event);
	void scrollVertically(INPUT_RECORD &event);
	void scrollHorizontally(INPUT_RECORD &event);
	void checkKeyEvent(INPUT_RECORD &event);
	std::string getNewPath(int index);
	std::string getNewPath(const std::string &name);
	void startFolderOpeningProcess(const std::string &newPath);
	void openFolder(int index);
	void openFolder(std::string path);
	void performFilesPartEvents(INPUT_RECORD &event);
	void performOptionsEvents(INPUT_RECORD &event);
	void performMouseEventsDuringRenameProcess(const INPUT_RECORD &event);
	void performSearchTableEvents(INPUT_RECORD &event);
	void performSearchResultsEvents(INPUT_RECORD &event);
	void startRenaming();
	void handleFilesPartEventsWhenLeftMouseButtonPressed(ChoosableButton &button, INPUT_RECORD &event);
	void handleEventsWhenRightMouseButtonPressedOfFilesPart(ChoosableButton &button, INPUT_RECORD &event);
	void drawItemsAccordingToStates();
	void handleEventsWhenMouseIsNotOnItems(ChoosableButton &button, INPUT_RECORD &event);
	std::vector<std::string> getContentOfFolder(std::string path);
	void doRenameOperations();
	void savePathes();
	std::string getFileOrFolderName(std::string &oldPath);
	void refreshItems();
};

#endif // Prog