#include "SearchTable.h"

SearchTable::SearchTable() {
	
	searchHeader.setTextAndColor("    Search     ");
	setStartPosition({ 0, 0 });
	setMinLength();
}


void SearchTable::setStartPosition(COORD start) {
	startPosition = start;
	searchHeader.setPosition(start);
	searchInput.setPosition({ start.X, start.Y + 1 });
	searchResults.setStartPosition({ 0, static_cast<short>(start.Y + 5) });
}

void SearchTable::setMinLength() {
	if (searchHeader.getTextString().size() > searchInput.getTextString().size()) {
		minLength = searchHeader.getTextString().size();
		searchHeader.setMinLength(minLength);
		searchInput.setMinLength(minLength);
	}
	else {
		minLength = searchInput.getTextString().size();
		searchHeader.setMinLength(minLength);
		searchInput.setMinLength(minLength);
	}
}

void SearchTable::appearOnConsole(HANDLE &hndl) {
	searchHeader.appearOnConsoleScreen(hndl);
	searchInput.appearOnConsoleScreen(hndl);
}



ChoosableButton &SearchTable::getSearchHeader() {
	return searchHeader;
}

InputForm &SearchTable::getSearchInput() {
	return searchInput;
}

std::vector<std::string> SearchTable::getResultStringsThroughIterating(const std::string &path) {
	if (searchInput.getTextString() == "") {
		throw std::runtime_error(noMaskInSearch);
	}
	std::string searchPath;
	if (path == "C:" || path == "D:") {
		searchPath = path + "/";
	}
	else {
		searchPath = path;
	}
	std::string mask = searchInput.getTextString();
	mask.erase(mask.end() - 1);
	std::vector<std::string> searchResults;
	for (auto &p : fs::recursive_directory_iterator(searchPath)) {
		if (HelperFunctions::checkMask(p.path().string(), mask)) {
			searchResults.push_back(p.path().string());
		}
	}
	searchResults.insert(searchResults.begin(), "..");
	return searchResults;
	
}



void SearchTable::search(const std::string &path) {
	std::vector<std::string> resultStrings = getResultStringsThroughIterating(path);
	searchResults.setMenuItems(resultStrings);
}

Menu &SearchTable::getSearchResults() {
	return searchResults;
}