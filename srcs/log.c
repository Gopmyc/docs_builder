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

void	logError(const char *sMsg, const char *sDetail)
{
	setConsoleColor(COLOR_ERROR);
	fprintf(stderr, "[ERROR] %s: %s\n", sMsg, sDetail ? sDetail : "N/A");
	setConsoleColor(COLOR_RESET);
}

void	logInfo(const char *sMsg, const char *sDetail)
{
	setConsoleColor(COLOR_INFO);
	printf("[INFO]  %s: %s\n", sMsg, sDetail ? sDetail : "...");
	setConsoleColor(COLOR_RESET);
}

void	logSuccess(const char *sMsg, const char *sDetail)
{
	setConsoleColor(COLOR_SUCCESS);
	printf("[OK]    %s: %s\n", sMsg, sDetail ? sDetail : "Done");
	setConsoleColor(COLOR_RESET);
}
