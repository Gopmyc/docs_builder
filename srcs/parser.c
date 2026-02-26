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

static char* read_multiline_block(FILE* f, char* first_line)
{
	char buf[1024];
	size_t alloc_size = 1024;
	char* str = malloc(alloc_size);
	str[0] = 0;

	char* line = first_line;
	int done = 0;

	while (!done && line)
	{
		char* start = line;
		char* brace = strchr(line, '{');
		if (brace) start = brace + 1;

		char* end = strchr(start, '}');
		if (end) { *end = 0; done = 1; }

		char* p = start;
		while (*p)
		{
			if (*p == '\n' || *p == '\r')
				strcat(str, "<br>");
			else
			{
				size_t len = strlen(str);
				if (len + 2 > alloc_size)
				{
					alloc_size *= 2;
					str = realloc(str, alloc_size);
				}
				str[len] = *p;
				str[len+1] = 0;
			}
			p++;
		}

		if (done) break;

		if (!fgets(buf, sizeof(buf), f))
			break;

		line = buf;
	}

	return str;
}

static void read_field_with_optional_multiline(FILE* f, char* line, char* colon, char* dest, size_t dest_size)
{
	char* val = colon + 1;
	while (*val == ' ' || *val == '\t') val++;

	if (*val != '\0' && *val != '\n' && *val != '\r' && *val != '{')
	{
		strncpy(dest, val, dest_size - 1);
		dest[dest_size - 1] = '\0';
		trim_trailing(dest);
		return;
	}

	if (*val == '\0' || *val == '\n' || *val == '\r')
	{
		if (!fgets(line, MAX_LINE, f)) return;
		val = line;
		while (*val == ' ' || *val == '\t' || *val == '\n' || *val == '\r') val++;
	}

	if (*val == '{' || strchr(val, '{'))
	{
		char* multi = read_multiline_block(f, val);
		strncpy(dest, multi, dest_size - 1);
		dest[dest_size - 1] = '\0';
		free(multi);
		trim_trailing(dest);
	}
	else
	{
		strncpy(dest, val, dest_size - 1);
		dest[dest_size - 1] = '\0';
		trim_trailing(dest);
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

		if (line[0] != '@') continue;
		char* colon = strchr(line, ':');
		if (!colon) continue;

		if (strncmp(line, "@GLOBAL_DESCRIPTION", 19) == 0)
			read_field_with_optional_multiline(f, line, colon, parsed->global_description, sizeof(parsed->global_description));

		else if (strncmp(line, "@NAME", 5) == 0)
		{
			current = calloc(1, sizeof(DocBlock));
			append_block(parsed, current);

			char* val = colon + 1;
			while (*val == ' ' || *val == '\t') val++;
			strncpy(current->name, val, sizeof(current->name) - 1);
			trim_trailing(current->name);
		}

		else if (strncmp(line, "@STATE", 6) == 0 && current)
			read_field_with_optional_multiline(f, line, colon, current->state, sizeof(current->state));

		else if (strncmp(line, "@DESC", 5) == 0 && current)
			read_field_with_optional_multiline(f, line, colon, current->desc, sizeof(current->desc));

		else if (strncmp(line, "@PARAM", 6) == 0 && current)
		{
			while (fgets(line, sizeof(line), f))
			{
				if (line[0] == '\n' || line[0] == '@') break;
				char* o = strchr(line, '{');
				char* c = strchr(line, '}');
				char* col = strchr(line, ':');
				if (!o || !c || !col) continue;

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
				if (line[0] == '\n' || line[0] == '@') break;
				char* o = strchr(line, '{');
				char* c = strchr(line, '}');
				char* col = strchr(line, ':');
				if (!o || !c || !col) continue;

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