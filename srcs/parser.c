#include "docs_builder.h"

//
// ┌────────────────────────┐
// │ DOCS GENERATION PARSER │
// └────────────────────────┘
//
static void trim_trailing(char* str)
{
	size_t len = strlen(str);
	while (len && (str[len - 1] == '\n' || str[len - 1] == '\r' || str[len - 1] == ' ' || str[len - 1] == '\t'))
		str[--len] = 0;
}
