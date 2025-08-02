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
