#ifndef HelpFun
#define HelpFun

#include <sys/stat.h>
#include <string>
#include <vector>

class HelperFunctions {
public:
	static bool is_dir(const char* path);
	static std::vector<std::string> createArrayOfSubStrings(std::string str, char delimeter);
};



#endif //HelpFun
