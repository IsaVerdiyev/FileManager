#include "isOnIndentationException.h"


isOnIndentationException::isOnIndentationException(int i, std::string error_message) 
	: std::runtime_error(error_message), index{ i }{}

int isOnIndentationException::getIndex() {
	return index;
}