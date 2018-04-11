#ifndef HelpFun
#define HelpFun

#include <sys/stat.h>
#include <string>
#include <vector>
#include <fstream>

class HelperFunctions {
public:
	static std::vector<std::string> split(std::string str, char delimeter);
	static bool checkMask(const std::string path, const std::string mask);
	static std::vector<std::string> getTextFromFile(std::ifstream &file);
	static void setTextInNewFile(std::ofstream &file, std::vector<std::string> &Text);
};



#endif //HelpFun
