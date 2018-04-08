#ifndef HelpFun
#define HelpFun

#include <sys/stat.h>
#include <string>
#include <vector>

class HelperFunctions {
public:
	static std::vector<std::string> split(std::string str, char delimeter);
	static bool checkMask(const std::string path, const std::string mask);
};



#endif //HelpFun
