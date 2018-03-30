
#include "HelperFunctions.h"




std::vector<std::string> HelperFunctions::createArrayOfSubStrings(std::string str, char delimeter) {
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