#ifndef SeaTab

#include "Menu.h"
#include "InputForm.h"
#include <string>
#include <experimental\filesystem>
#include "HelperFunctions.h"
#include "ErrorStrings.h"

namespace fs = std::experimental::filesystem;


class SearchTable {
	int minLength;
	COORD startPosition;
	ChoosableButton searchHeader;
	InputForm searchInput;
	Menu searchResults;
public:
	SearchTable();
	ChoosableButton &getSearchHeader();
	InputForm &getSearchInput();
	void setStartPosition(COORD start);
	void setMinLength();
	void appearOnConsole(HANDLE &hndl);
	std::vector<std::string> getResultStringsThroughIterating(const std::string &path);
	void search(const std::string &path);
	Menu &getSearchResults();
};





#endif //SeaTab
