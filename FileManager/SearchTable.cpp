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
	if (searchHeader.getText().size() > searchInput.getText().size()) {
		minLength = searchHeader.getText().size();
		searchHeader.setMinLength(minLength);
		searchInput.setMinLength(minLength);
	}
	else {
		minLength = searchInput.getText().size();
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

std::vector<std::string> SearchTable::getResultStringsThroughIterating(const std::string &path, const std::vector<std::string> &disks) {
	if (searchInput.getTextString() == "") {
		throw std::runtime_error(noMaskInSearch);
	}
	std::vector<std::string> searchResults;
	std::string mask = searchInput.getTextString();
	std::string searchPath = getOptimizedSearchPath(path);

	if (searchPath == "") {
		for (int i = 0; i < disks.size(); i++) {
			std::vector<std::string> searchResultsFromOneDisk = getResultStringsThroughIterating(disks[i], disks);
			for (int i = 0; i < searchResultsFromOneDisk.size(); i++) {
				searchResults.push_back(searchResultsFromOneDisk[i]);
			}
		}
		return searchResults;
	}
	else if (searchPath.find('\\') == std::string::npos) {
		searchPath = searchPath + "\\";
	}
	for (auto &p : fs::recursive_directory_iterator(searchPath)) {
		try {
			if (HelperFunctions::checkMask(p.path().string(), mask)) {
				searchResults.push_back(p.path().string());
			}
		}
		catch (std::exception ex) {
			continue;
		}
	}
	searchResults.insert(searchResults.begin(), "..");
	return searchResults;
	
}



void SearchTable::search(const std::string &path, const std::vector<std::string> &disks) {
	std::vector<std::string> resultStrings = getResultStringsThroughIterating(path, disks);
	searchResults.setMenuItems(resultStrings);
}

Menu &SearchTable::getSearchResults() {
	return searchResults;
}

std::string SearchTable::getOptimizedSearchPath(const std::string &path) {
	if (searchInput.getTextString()[0] == '*') {
		return path;
	}
	else {
		std::vector<std::string> maskParts = HelperFunctions::split(searchInput.getTextString(), '*'); 
		size_t pos = maskParts[0].find_last_of('\\');
		if (pos == std::string::npos) {
			return path;
		}
		else {
			maskParts[0].erase(pos);
			if (HelperFunctions::checkMask(maskParts[0], path + "*")) {
				return maskParts[0];
			}
		}
	}
}