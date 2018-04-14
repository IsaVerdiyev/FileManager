#include "isNextAfterIndentation.h"


isNextAfterIndentationException::isNextAfterIndentationException(int i, std::string error_message) 
	:runtime_error(error_message), index {i} {}

int isNextAfterIndentationException::getIndex() {
	return index;
}