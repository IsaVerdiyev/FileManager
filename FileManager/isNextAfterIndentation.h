#ifndef AftIndExc
#define AftIndExc

#include <stdexcept>

class isNextAfterIndentationException : public std::runtime_error {
	int index;
public:
	isNextAfterIndentationException(int i, std::string error_message = "");
	int getIndex();
};









#endif //AftIndExc
