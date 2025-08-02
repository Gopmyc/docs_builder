#include "docs_builder.h"

//
// ┌────────────┐
// │ LOG SYSTEM │
// └────────────┘
//
void	setConsoleColor(WORD wColor)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, wColor);
}
