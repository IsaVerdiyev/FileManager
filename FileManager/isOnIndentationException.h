#ifndef Exc
#define Exc

/*
	Exception class which specifies if index is on spaces of \t.
*/

#include <stdexcept>

class isOnIndentationException : public std::runtime_error {
	int index; // index of \t in string
public:
	isOnIndentationException(int i, std::string error_message = "");
	int getIndex();
};







#endif
