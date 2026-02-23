#include "docs_builder.h"

//
// ┌────────────┐
// │ LOG SYSTEM │
// └────────────┘
//

static RuntimeConfig* g_runtime;

void set_runtime_config(RuntimeConfig* runtime)
{
	g_runtime = runtime;
}

void	setConsoleColor(WORD wColor)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, wColor);
}

void	logError(const char *sMsg, const char *sDetail)
{
	if (g_runtime)
		setConsoleColor(g_runtime->color_error);

	fprintf(stderr, "[ERROR] %s: %s\n", sMsg, sDetail ? sDetail : "N/A");

	if (g_runtime)
		setConsoleColor(g_runtime->color_reset);
}

void	logInfo(const char *sMsg, const char *sDetail)
{
	if (g_runtime)
		setConsoleColor(g_runtime->color_info);

	printf("[INFO]  %s: %s\n", sMsg, sDetail ? sDetail : "...");

	if (g_runtime)
		setConsoleColor(g_runtime->color_reset);
}

void	logSuccess(const char *sMsg, const char *sDetail)
{
	if (g_runtime)
		setConsoleColor(g_runtime->color_success);

	printf("[OK]    %s: %s\n", sMsg, sDetail ? sDetail : "Done");
	
	if (g_runtime)
		setConsoleColor(g_runtime->color_reset);
}
