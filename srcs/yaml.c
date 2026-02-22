#include "docs_builder.h"

//
// ┌─────────────┐
// │ YAML PARSER │
// └─────────────┘
//

static char* ltrim(char* s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return s;
}

static void rtrim(char* s)
{
	size_t len = strlen(s);
	while (len > 0 &&
		(s[len - 1] == '\n' ||
		 s[len - 1] == '\r' ||
		 s[len - 1] == ' '  ||
		 s[len - 1] == '\t'))
	{
		s[--len] = '\0';
	}
}

static void strip_quotes(char* s)
{
	size_t len = strlen(s);
	if (len >= 2 && s[0] == '"' && s[len - 1] == '"')
	{
		memmove(s, s + 1, len - 2);
		s[len - 2] = '\0';
	}
}

static void assign_root_key(ProjectConfig* cfg, const char* key, const char* value)
{
	if (strcmp(key, "TITLE") == 0)
		strncpy(cfg->title, value, sizeof(cfg->title) - 1);
	else if (strcmp(key, "DESCRIPTION") == 0)
		strncpy(cfg->description, value, sizeof(cfg->description) - 1);
}

static void assign_child_key(ProjectConfig* cfg, const char* parent, const char* key, const char* value)
{
	if (strcmp(parent, "LICENSE") == 0)
	{
		if (strcmp(key, "NAME") == 0)
			strncpy(cfg->license_name, value, sizeof(cfg->license_name) - 1);
		else if (strcmp(key, "URL") == 0)
			strncpy(cfg->license_url, value, sizeof(cfg->license_url) - 1);
	}
	else if (strcmp(parent, "THEME") == 0)
	{
		if (strcmp(key, "PRIMARY_COLOR") == 0)
			strncpy(cfg->theme.primary_color, value, sizeof(cfg->theme.primary_color) - 1);
		else if (strcmp(key, "SIDEBAR_TITLE") == 0)
			strncpy(cfg->theme.sidebar_title, value, sizeof(cfg->theme.sidebar_title) - 1);
	}
}

int load_config_yaml(const char* path, ProjectConfig* config)
{
	FILE* f = fopen(path, "r");
	if (!f)
		return 0;

	memset(config, 0, sizeof(ProjectConfig));

	char line[512];
	char current_parent[128] = {0};

	while (fgets(line, sizeof(line), f))
	{
		rtrim(line);

		char* trimmed = ltrim(line);

		if (*trimmed == '\0' || *trimmed == '#')
			continue;

		int indent = (int)(trimmed - line);

		char* colon = strchr(trimmed, ':');
		if (!colon)
			continue;

		*colon = '\0';
		char* key = trimmed;
		char* value = colon + 1;

		value = ltrim(value);
		strip_quotes(value);

		if (indent == 0)
		{
			if (*value == '\0')
			{
				strncpy(current_parent, key, sizeof(current_parent) - 1);
			}
			else
			{
				assign_root_key(config, key, value);
				current_parent[0] = '\0';
			}
		}
		else
		{
			if (current_parent[0] != '\0')
				assign_child_key(config, current_parent, key, value);
		}
	}

	fclose(f);
	return 1;
}