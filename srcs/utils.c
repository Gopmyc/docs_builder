#include "docs_builder.h"

//
// ┌───────────────┐
// │ UTILITY FUNCS │
// └───────────────┘
//
void	writeIndent(FILE *fOut, int iDepth)
{
	for (int i = 0; i < iDepth * INDENT_WIDTH; i++)
		fputc('\t', fOut);
}

int	endsWithHtml(const char *sName)
{
	size_t iLen = strlen(sName);
	size_t iExtLen = strlen(EXTENSION);

	return iLen >= iExtLen &&
		strcmp(sName + iLen - iExtLen, EXTENSION) == 0 &&
		strcmp(sName, EXCLUDE_FILE) != 0;
}

int	isDirectory(const char *sPath)
{
	struct _stat sStat;
	return _stat(sPath, &sStat) == 0 && (sStat.st_mode & _S_IFDIR);
}

