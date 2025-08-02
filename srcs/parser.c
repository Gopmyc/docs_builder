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

DocBlock* parse_doc_blocks(FILE* f)
{
	char line[MAX_LINE];
	int inBlock = 0;
	DocBlock* head = NULL;
	DocBlock* current = NULL;

	while (fgets(line, sizeof(line), f)) {
		if (!inBlock) {
			if (strstr(line, "--[[")) {
				inBlock = 1;
				DocBlock* newBlock = (DocBlock*)calloc(1, sizeof(DocBlock));
				if (!head) head = newBlock;
				if (current) current->next = newBlock;
				current = newBlock;
			}
		} else {
			if (strstr(line, "]]")) { inBlock = 0; current = NULL; }
			else if (current) {
				char* p;

				if ((p = strstr(line, "@name"))) {
					char* colon = strchr(p, ':');
					if (colon) {
						++colon; while (*colon == ' ' || *colon == '\t') ++colon;
						strncpy(current->name, colon, sizeof(current->name) - 1);
						trim_trailing(current->name);
					}
				}

				if ((p = strstr(line, "@desc"))) {
					char* colon = strchr(p, ':');
					if (colon) {
						++colon; while (*colon == ' ' || *colon == '\t') ++colon;
						strncpy(current->desc, colon, sizeof(current->desc) - 1);
						trim_trailing(current->desc);
					}
				}

				if ((p = strstr(line, "@param"))) {
					Param* param = (Param*)calloc(1, sizeof(Param));
					char* o = strchr(p, '{'), *c = strchr(p, '}'), *colon = strchr(p, ':');
					if (o && c && colon && o < c && c < colon) {
						size_t len = c - o - 1;
						if (len >= sizeof(param->type)) len = sizeof(param->type) - 1;
						strncpy(param->type, o + 1, len);
						param->type[len] = '\0';
						colon++;
						while (*colon == ' ' || *colon == '\t') ++colon;
						strncpy(param->desc, colon, sizeof(param->desc) - 1);
						trim_trailing(param->desc);
					}
					if (!current->params) current->params = param;
					else {
						Param* last = current->params;
						while (last->next) last = last->next;
						last->next = param;
					}
				}

				if ((p = strstr(line, "@return"))) {
					Return* ret = (Return*)calloc(1, sizeof(Return));
					char* o = strchr(p, '{'), *c = strchr(p, '}'), *colon = strchr(p, ':');
					if (o && c && colon && o < c && c < colon) {
						size_t len = c - o - 1;
						if (len >= sizeof(ret->type)) len = sizeof(ret->type) - 1;
						strncpy(ret->type, o + 1, len);
						ret->type[len] = '\0';
						colon++;
						while (*colon == ' ' || *colon == '\t') ++colon;
						strncpy(ret->desc, colon, sizeof(ret->desc) - 1);
						trim_trailing(ret->desc);
					}
					if (!current->returns) current->returns = ret;
					else {
						Return* last = current->returns;
						while (last->next) last = last->next;
						last->next = ret;
					}
				}
			}
		}
	}
	return head;
}
