#ifndef SeaTab


/*
	This class performs functioanility of searching files and folders specified in 
	inputForm object of this class
*/

#include "Menu.h"
#include "InputForm.h"
#include <experimental\filesystem>
#include "HelperFunctions.h"
#include "ErrorStrings.h"

namespace fs = std::experimental::filesystem;


class SearchTable {
	int minLength;	// minimumLength of objects of this class
	COORD startPosition; // positions of displaying objects
	ChoosableButton searchHeader;  // button of this class
	InputForm searchInput; // object that takes input
	Menu searchResults; // objects that will be displayed in result of success of search
public:
	SearchTable();
	ChoosableButton &getSearchHeader();
	InputForm &getSearchInput();
	void setStartPosition(COORD start);
	void setMinLength();
	void appearOnConsole(HANDLE &hndl); // draws button and input objects


	std::vector<std::string> getResultStringsThroughIterating(const std::string &path, const std::vector<std::string> &disks);
	// performs searching  throug iterating


	void search(const std::string &path, const std::vector<std::string> &disks);
	// function responsible for start searching and writing results in 'searchResults' variable
	
	Menu &getSearchResults(); // returns searchResults
	std::string getOptimizedSearchPath(const std::string &path);
	// function makes some changes to path where to search in optimization purposes in
	// some special cases
};





#endif //SeaTab
