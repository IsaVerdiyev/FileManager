#include "InputForm.h"
#include "ChoosableButton.h"
#include "Menu.h"
#include "Program.h"


int main() {
	Program pr;
	pr.performCycleOfEvents();
}
//#include "Program.h";
//
//
//int main()
//{
//	HANDLE wHndl = GetStdHandle(STD_OUTPUT_HANDLE);
//	
//	DiskMenu dm;
//	dm.setStartPosition({ 5, 5 });
//	dm.drawMenu(wHndl);
//	system("pause");
//}