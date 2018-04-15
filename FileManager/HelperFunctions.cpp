
#include "HelperFunctions.h"




std::vector<std::string> HelperFunctions::split(std::string str, char delimeter) {
	std::vector<std::string> stringArray;
	int prevCounter = 0;
	for (int curCounter = 0; curCounter <= str.length(); curCounter++) {
		if (str[curCounter] == delimeter || str[curCounter] == 0) {
			std::string word = str.substr(prevCounter, curCounter - prevCounter);
			prevCounter = curCounter + 1;
			std::string check = "";
			check.push_back(delimeter);
			if (word == check || word == "") {
				continue;
			}
			else {
				stringArray.push_back(word);
			}
		}
	}
	return stringArray;
}


bool HelperFunctions::checkMask(const std::string path, const std::string mask) {
	size_t pos = 0;
	std::vector<std::string> partsOfMask = split(mask, '*');
	try {
		if (mask[0] != '*') {
			if (path.compare(0, partsOfMask[0].size(), partsOfMask[0])) {
				return false;
			}

		}
		if (mask[mask.size() - 1] != '*') {
			if (path.compare(path.size() - partsOfMask[partsOfMask.size() - 1].size(), partsOfMask[partsOfMask.size() - 1].size(), partsOfMask[partsOfMask.size() - 1])) {
				return false;
			}
		}
	}
	catch (std::out_of_range ex) {
		return false;
	}
	for (int i = 0; i < partsOfMask.size(); i++) {
		pos = path.find(partsOfMask[i], pos);
		if (pos == std::string::npos) {
			return false;
		}
		pos + 1;
	}
	return true;
}

std::vector<std::string> HelperFunctions::getTextFromFile(std::ifstream &file) {
	std::vector<std::string> linesOfText;
	while (!file.eof()) {
		std::string newLine;
		getline(file, newLine);
		linesOfText.push_back(newLine);
	}
	return linesOfText;
}


void HelperFunctions::setTextInNewFile(std::ofstream &file, std::vector<std::string> &Text) {
	for (int i = 0; i < Text.size(); i++) {
		file << Text[i];
		if (i != Text.size() - 1) {
			file << "\n";
		}
	}
}

std::string HelperFunctions::getStringWithReplacedSlashT_ToSpaces(const std::string &str, int slashT_SpaceCounts, std::vector<int> *slashT_positions) {
	std::string s;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '\t') {
			if (slashT_positions) {
				slashT_positions->push_back(i);
			}
			for (int j = 0; j < slashT_SpaceCounts; j++) {
				s.push_back(' ');
			}
		}
		else {
			s.push_back(str[i]);
		}
	}
	return s;
}
