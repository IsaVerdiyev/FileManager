#ifndef TL
#define TL

#include <string>
#include <vector>
#include <Windows.h>
#include <memory>
#include "Colors.h"
#include "HelperFunctions.h"

class TextLine {
protected:
	bool isOnScreen;
	bool positionChanged;
	bool lengthChanged;
	COORD startPosition;
	std::vector<CHAR_INFO> sentenceSymbols;
	std::vector<CHAR_INFO> eraseArray;
	std::string textInLine;
	std::vector<int> slashT_positions;
	COORD oldStartPosition;
	void setSymbols();
	int minLength = 1;
	int stringSize = 0;
	void createEraseArray(HANDLE &hndl);
	void resizeAccordingToMinLength();
public:
	static const int slashT_SpaceCounts;
	TextLine();
	TextLine(const std::string &s, Color c = SameColor);
	void setColor(Color c = SameColor);
	void setPosition(COORD pos);
	void setTextAndColor(const std::string &s, Color c = SameColor);
	std::vector<CHAR_INFO> &getSentenceSymbols();
	void setMinLength(int length);
	void putCharInfoArrayInConsoleBuffer(HANDLE &hndl, std::vector<CHAR_INFO> &symbolsArray, COORD pos);
	void appearOnConsoleScreen(HANDLE &hndl, COORD beginningPosition);
	virtual void appearOnConsoleScreen(HANDLE &hndl);
	void removeFromConsoleScreen(HANDLE &hndl);
	COORD getEndPosition();
	bool isMouseOnButton(const INPUT_RECORD &event);
	bool getIsOnScreenState();
	int getMinLength();
	COORD getStartPosition();
	std::string getTextInLine();
};


#endif // !TL

