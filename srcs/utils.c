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

//
// ┌───────────────────────┐
// │ DOCS GENERATION UTILS │
// └───────────────────────┘
//
void free_docblocks(DocBlock* head)
{
	while (head) {
		DocBlock* tmp = head->next;
		Param* p = head->params;
		while (p) { Param* ptmp = p->next; free(p); p = ptmp; }

		Return* r = head->returns;
		while (r) { Return* rtmp = r->next; free(r); r = rtmp; }

		free(head); head = tmp;
	}
}

