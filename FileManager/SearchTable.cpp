#include "SearchTable.h"

SearchTable::SearchTable() {
	
	searchHeader.setTextAndColor("    Search     ", defaultColor);
	setStartPosition({ 0, 0 });
	setMinLength();
}

void SearchTable::setStartPosition(COORD start) {
	startPosition = start;
	searchHeader.setPosition(start);
	searchInput.setPosition({ start.X, start.Y + 1 });
}

void SearchTable::setMinLength() {
	if (searchHeader.getTextString().size() > searchInput.getTextString().size()) {
		searchHeader.setMinLength(searchHeader.getTextString().size());
		searchInput.setMinLength(searchHeader.getTextString().size());
	}
	else {
		searchHeader.setMinLength(searchInput.getTextString().size());
		searchInput.setMinLength(searchInput.getTextString().size());
	}
}

void SearchTable::appearOnConsole(HANDLE &hndl) {
	searchHeader.appearOnConsoleScreen(hndl);
	searchInput.appearOnConsoleScreen(hndl);
}

bool SearchTable::isMouseOnSearch(const INPUT_RECORD &event) {
	return searchInput.isMouseOnButton(event);
}

void SearchTable::takeInput(const INPUT_RECORD &event) {
	searchInput.takeInput(event);
	setMinLength();
}
bool SearchTable::isGettingInput() {
	return searchInput.isGettingInput();
}
void SearchTable::setActiveColor(Color c) {
	searchInput.setActiveColor(c);
}
void SearchTable::setDeactiveColor(Color c) {
	searchInput.setDeactiveColor(c);
}
bool SearchTable::turnInputStateOn() {
	return searchInput.turnInputStateOn();
}
bool SearchTable::turnInputStateOff() {
	return searchInput.turnInputStateOff();
}