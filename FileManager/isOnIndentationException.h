#ifndef Exc
#define Exc

#include <stdexcept>

class isOnIndentationException : public std::runtime_error {
	int index;
public:
	isOnIndentationException(int i, std::string error_message);
	int getIndex();
};







#endif
