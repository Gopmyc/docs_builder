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

static void append_block(ParsedFile* file, DocBlock* block)
{
	if (!file->blocks)
		file->blocks = block;
	else
	{
		DocBlock* tmp = file->blocks;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = block;
	}
}

static void append_param(DocBlock* block, Param* p)
{
	if (!block->params)
		block->params = p;
	else
	{
		Param* tmp = block->params;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = p;
	}
}

static void append_return(DocBlock* block, Return* r)
{
	if (!block->returns)
		block->returns = r;
	else
	{
		Return* tmp = block->returns;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = r;
	}
}

ParsedFile* parse_doc_file(FILE* f)
{
	char line[MAX_LINE];
	ParsedFile* parsed = calloc(1, sizeof(ParsedFile));
	DocBlock* current = NULL;
	int in_example = 0;

	while (fgets(line, sizeof(line), f))
	{
		if (in_example)
		{
			if (strncmp(line, "```", 3) == 0)
			{
				in_example = 0;
				continue;
			}
			strcat(current->example_code, line);
			continue;
		}

		if (line[0] != '@')
			continue;

		char* colon = strchr(line, ':');
		if (!colon)
			continue;

		if (strncmp(line, "@GLOBAL_DESCRIPTION", 19) == 0)
		{
			while (fgets(line, sizeof(line), f))
			{
				if (line[0] == '\n' || line[0] == '@')
					break;
				strcat(parsed->global_description, line);
			}
		}
		else if (strncmp(line, "@NAME", 5) == 0)
		{
			current = calloc(1, sizeof(DocBlock));
			append_block(parsed, current);

			strncpy(current->name, colon + 1, sizeof(current->name) - 1);
			trim_trailing(current->name);
		}
		else if (strncmp(line, "@DESC", 5) == 0 && current)
		{
			strncpy(current->desc, colon + 1, sizeof(current->desc) - 1);
			trim_trailing(current->desc);
		}
		else if (strncmp(line, "@PARAM", 6) == 0 && current)
		{
			while (fgets(line, sizeof(line), f))
			{
				if (line[0] == '\n' || line[0] == '@')
					break;

				char* o = strchr(line, '{');
				char* c = strchr(line, '}');
				char* col = strchr(line, ':');

				if (!o || !c || !col)
					continue;

				Param* p = calloc(1, sizeof(Param));

				size_t len = c - o - 1;
				strncpy(p->type, o + 1, len);
				p->type[len] = 0;

				strncpy(p->desc, col + 1, sizeof(p->desc) - 1);
				trim_trailing(p->desc);

				append_param(current, p);
			}
		}
		else if (strncmp(line, "@RETURN", 7) == 0 && current)
		{
			while (fgets(line, sizeof(line), f))
			{
				if (line[0] == '\n' || line[0] == '@')
					break;

				char* o = strchr(line, '{');
				char* c = strchr(line, '}');
				char* col = strchr(line, ':');

				if (!o || !c || !col)
					continue;

				Return* r = calloc(1, sizeof(Return));

				size_t len = c - o - 1;
				strncpy(r->type, o + 1, len);
				r->type[len] = 0;

				strncpy(r->desc, col + 1, sizeof(r->desc) - 1);
				trim_trailing(r->desc);

				append_return(current, r);
			}
		}
		else if (strncmp(line, "@EXAMPLE", 8) == 0 && current)
		{
			if (fgets(line, sizeof(line), f))
			{
				if (strncmp(line, "```", 3) == 0)
				{
					strncpy(current->example_lang, line + 3, sizeof(current->example_lang) - 1);
					trim_trailing(current->example_lang);
					in_example = 1;
				}
			}
		}
	}

	return parsed;
}

void free_parsed_file(ParsedFile* file)
{
	if (!file) return;

	DocBlock* b = file->blocks;
	while (b)
	{
		Param* p = b->params;
		while (p) { Param* np = p->next; free(p); p = np; }

		Return* r = b->returns;
		while (r) { Return* nr = r->next; free(r); r = nr; }

		DocBlock* nb = b->next;
		free(b);
		b = nb;
	}

	free(file);
}