#ifndef HelpFun
#define HelpFun

#include <sys/stat.h>
#include <string>
#include <vector>
#include <fstream>

class HelperFunctions {
public:
	static std::vector<std::string> split(std::string str, char delimeter);
	// splits string by delimiter

	static bool checkMask(const std::string path, const std::string mask);
	// checks if mask is suitable for this path

	static std::vector<std::string> getTextFromFile(std::ifstream &file);
	// function returns vector of strings which are got from file

	static void setTextInNewFile(std::ofstream &file, std::vector<std::string> &Text);
	// function writes vector of strings in file

	static std::string getStringWithReplacedSlashT_ToSpaces(const std::string &str, int slashT_SpaceCounts, std::vector<int> *slashT_positions = nullptr);
	// function returns string where after \t there are additional spaces in amount of value of slashT_SpaceCounts paramater
};



#endif //HelpFun
