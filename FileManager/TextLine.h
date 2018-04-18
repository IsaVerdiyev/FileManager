#ifndef TL
#define TL

/*
	This class is designed for displaying string on Console through putting CHAR_INFO array 
	buffer of Console.
*/

#include <string>
#include <vector>
#include <Windows.h>
#include <memory>
#include "Colors.h"
#include "HelperFunctions.h"

class TextLine {
protected:
	bool isOnScreen;  // bool variable for knowing if object's string is already on display
	bool positionChanged;  // bool variable for knowing if object's position changed after last its appearing on Screen
	bool lengthChanged;		// bool variable for checking if length of string on Console changed
	COORD startPosition;	// variable which contains point on Console to start drawing
	std::vector<CHAR_INFO> sentenceSymbols;	// array of CHAR_INFO which program will display through puttin in buffer of Console
	
	std::vector<CHAR_INFO> eraseArray; // in this array of CHAR_INFO is saved 
	//previous CHAR_INFO array of Console buffer before displaying.
	// When program will need to  erase object's string from screen, 
	//it will put this  erase array back in Console buffer;

	std::string textInLine; // string this object contains

	COORD oldStartPosition; // previous position of displaying this object on Screen.
	// it is used when need to erase last displaying of string from Console and display 
	// in other pothisition of Console. 

	void setSymbols(); // this function creates array of CHAR_INFO that must be displayed.
	// Symbols are got accordint to string that object contains.

	int minLength = 1; // variable which  shows minimum length that should be displayed on 
	// screen. Even if string size is less than minimum length, rest part is of CHAR_INFO
	// array is fullfilled with spaces till size of that array equals minimum length

	int stringSize = 0; // because there can be symbols of spaces in CHAR_INFO array that
	// are needed just for fullfilling all minimum length, real size of string on Screen 
	// is saved in this variable

	void resizeAccordingToMinLength(); // this function sets CHAR_INFO array length according
	// to new minimum length, when minimum length is changed

public:
	static const int slashT_SpaceCounts; // variable specifies how many spaces should display
	// program instead of \t

	TextLine(); // calls TextLine("")

	TextLine(const std::string &s, Color c = SameColor); // this constructor sets string, 
	// array of CHAR_INFO and color in that array if specified

	void setColor(Color c = SameColor); // function sets color of CHAR_INFO array
	void setPosition(COORD pos); // sets startPosition for display  and changes flag of positionChanged to true
	
	void setTextAndColor(const std::string &s, Color c = SameColor); // sets string, 
	// array of CHAR_INFO and color. (is used in Constructor)

	std::vector<CHAR_INFO> &getSentenceSymbols(); // returnes array of CHAR_INFO
	void setMinLength(int length); // sets minimum length of array of CHAR_INFO

	void putCharInfoArrayInConsoleBuffer(HANDLE &hndl, std::vector<CHAR_INFO> &symbolsArray, COORD pos);
	// function is responsible for putting array of CHAR_INFO in buffer of Console. By this way
	// program draws string or erases it

	void appearOnConsoleScreen(HANDLE &hndl, COORD beginningPosition); // draws array on Console in specified position
	virtual void appearOnConsoleScreen(HANDLE &hndl); // draws array on Console
	void removeFromConsoleScreen(HANDLE &hndl); // erases from Screen string
	COORD getEndPosition(); // returns end position of CHAR_INFO on Console
	bool isMouseOnButton(const INPUT_RECORD &event); // checks if mouse is on object
	bool getIsOnScreenState(); // checks if object is already displayed on screen
	int getMinLength(); // returns minimum length of CHAR_INFO array
	COORD getStartPosition(); // returns start position of displaying
	std::string getTextInLine(); // returns string in its original std::string form
	std::string getVisibleString(); // returns string that is converted for displaying (after \t there are spaces)
	void setVisibleStringSize(); // sets stringSize
	void createEraseArray(HANDLE &hndl); // creates eraseArray that will be needed when removing object from screen
};


#endif // !TL

