#ifndef SeaTab

#include "TemplateMenu.h"
#include "InputForm.h"
#include <string>
#include <experimental\filesystem>
#include "HelperFunctions.h"
#include "ErrorStrings.h"
#include "ChoosableButton.h"

namespace fs = std::experimental::filesystem;


class SearchTable {
	int minLength;
	COORD startPosition;
	ChoosableButton searchHeader;
	InputForm searchInput;
	TemplateMenu<ChoosableButton> searchResults;
public:
	SearchTable();
	ChoosableButton &getSearchHeader();
	InputForm &getSearchInput();
	void setStartPosition(COORD start);
	void setMinLength();
	void appearOnConsole(HANDLE &hndl);
	std::vector<std::string> getResultStringsThroughIterating(const std::string &path, const std::vector<std::string> &disks);
	void search(const std::string &path, const std::vector<std::string> &disks);
	TemplateMenu<ChoosableButton> &getSearchResults();
	std::string getOptimizedSearchPath(const std::string &path);
};





#endif //SeaTab
