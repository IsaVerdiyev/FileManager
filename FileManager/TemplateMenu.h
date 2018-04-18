#ifndef TempMen
#define TempMen

/*
	This class is template class designed for purposes of creating array of objects as TextLine
	and inherited from TextLine; and performing common functionality with them
*/

#include <vector>
#include <Windows.h>
#include <string>
#include "Colors.h"
#include "HelperFunctions.h"

template<class T>
class TemplateMenu {
protected:
	bool isOnScreen = false; // flag showing if this class's objects are already on screen
	bool lengthChanged = false; // flag shows if length of items changed
	bool positionChanged = false;	// flag shows if position changed
	bool amountOfLinesChanged = false;	// flag shows if amount of objects changed
	int lengthOfItem = 0; // shows length of eachItem
	std::vector<std::string> menuItemStrings; // array of strings of objects
	std::vector<T> menuItemButtons; // array of objects
	std::vector<T> eraseItemButtons;// this array is used to erase objects that are on screen
	COORD startPositionOfDisplay; // variable showing position of displaying object


	void setMenuItemStrings(const std::vector<std::string> &items) {
		menuItemStrings = items; 
	}

	void setMenuItemButtons() {
		setLengthOfItems(); // first is searched length of maximum string and it is saved in lengthOfItem
		menuItemButtons.clear(); // previous array of buttons is erased

		// here cycle of creating objects for displaying starts
		for (int i = 0; i < menuItemStrings.size(); i++) {
			menuItemButtons.push_back(T(menuItemStrings[i])); // add object with specified string in array of objects
			
			menuItemButtons[i].setPosition({ startPositionOfDisplay.X, static_cast<short>(startPositionOfDisplay.Y + i) });
			// sets position of newly added object in specified position

			menuItemButtons[i].setMinLength(lengthOfItem);
			// here we set minimum length of object to length of maximum string in array
		}
	}

	void setLengthOfItems() {
		// we search maximum length and specify if length changed in flag
		if (lengthOfItem != searchLengthOfItems()) {
			lengthChanged = true;
			lengthOfItem = searchLengthOfItems();
		}
	}

	int searchLengthOfItems() {
		// searching maximum length and returning it
		int maxLengthOfString = 0;
		for (std::string &str : menuItemStrings) {
			std::string visibleStr = HelperFunctions::getStringWithReplacedSlashT_ToSpaces(str, TextLine::slashT_SpaceCounts);
			if (maxLengthOfString < visibleStr.size() + 1) {
				maxLengthOfString = visibleStr.size() + 1;
			}
		}
		return maxLengthOfString;
	}


public:
	TemplateMenu() {};

	TemplateMenu(const std::vector<std::string> &itemsVector) {
		setMenuItems(itemsVector);
	}

	void setStartPosition(COORD pos) {
	// setting start position of displaying objects and position of each object according 
	// to new start position. Functions sets flag of positionChanged to true
		startPositionOfDisplay = pos;
		positionChanged = true;
		for (int i = 0; i < menuItemButtons.size(); i++) {
			menuItemButtons[i].setPosition({ pos.X, static_cast<short>(pos.Y + i) });
		}
	}

	void setMenuItems(const std::vector<std::string> &items) {
		// sets strings of objects in class
		setMenuItemStrings(items);
		// if amount of objects change, function sets appropriate flag to true
		if (items.size() != menuItemButtons.size()) {
			amountOfLinesChanged = true;
		}
		//creating objects
		setMenuItemButtons();
		
	}

	void removeMenuFromScreen(HANDLE &hndl) {
		//if objects of class are on screen, function removes them from screen
		if (isOnScreen) {
			for (T &button : menuItemButtons) {
				button.removeFromConsoleScreen(hndl);
			}
			isOnScreen = false; // after removing function sets flag 'isOnScreen' to false
		}
	}

	void drawMenu(HANDLE &hndl) { // functions draws objects on screen
		if (!isOnScreen) {// if objects are not on screen yet
			// function draws objects
			for (T &button : menuItemButtons) {
				button.appearOnConsoleScreen(hndl);
			}
			// here functions saves objects in eraseItemButtons for removing from screen
			// when it is needed in future
			eraseItemButtons = menuItemButtons;
			// immediately after drawing all change flags function sets to false
			lengthChanged = false;
			positionChanged = false;
			amountOfLinesChanged = false;
		}
		else if (isOnScreen && (lengthChanged || positionChanged || amountOfLinesChanged)) {
			// if objects are already on screen and some visual changes were made, function
			// shoud erase previous objects and only then draw new objects
			
			for (T &button : eraseItemButtons) { // erasing cycle
				button.removeFromConsoleScreen(hndl);
			}
			// setting change states to false
			lengthChanged = false;
			positionChanged = false;
			amountOfLinesChanged = false;
			for (T &button : menuItemButtons) { // drawing objects
				button.appearOnConsoleScreen(hndl);
			}
			// after drawing function makes backup copy of new items
			eraseItemButtons = menuItemButtons;
		}
		else {
			// if no special changes were made and is objects are on screen
			// functions just draws them above previous ones
			for (T &button : menuItemButtons) {
				button.appearOnConsoleScreen(hndl);
			}
		}

		// at the end function puts flag 'isOnScreen' to true
		isOnScreen = true;
	}

	std::vector<T> &getButtons() { // returns array of buttons
		return menuItemButtons;
	}

	std::vector<std::string> &getMenuItemStrings() { // returns array of strings
		return menuItemStrings;
	}

};


#endif // TempMen
