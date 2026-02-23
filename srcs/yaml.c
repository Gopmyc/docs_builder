#include "docs_builder.h"

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
		(s[len - 1] == '\n' || s[len - 1] == '\r' || s[len - 1] == ' ' || s[len - 1] == '\t'))
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
	else if (strcmp(key, "LICENSE_NAME") == 0)
		strncpy(cfg->license_name, value, sizeof(cfg->license_name) - 1);
	else if (strcmp(key, "LICENSE_URL") == 0)
		strncpy(cfg->license_url, value, sizeof(cfg->license_url) - 1);
}

static void assign_child_key(ProjectConfig* cfg, const char* parent, const char* key, const char* value)
{
	if (strcmp(parent, "THEME") == 0)
	{
		if (strcmp(key, "BG_MAIN") == 0)
			strncpy(cfg->theme.bg_main, value, sizeof(cfg->theme.bg_main) - 1);
		else if (strcmp(key, "BG_GRADIENT_START") == 0)
			strncpy(cfg->theme.bg_gradient_start, value, sizeof(cfg->theme.bg_gradient_start) - 1);
		else if (strcmp(key, "BG_GRADIENT_END") == 0)
			strncpy(cfg->theme.bg_gradient_end, value, sizeof(cfg->theme.bg_gradient_end) - 1);
		else if (strcmp(key, "BG_PANEL") == 0)
			strncpy(cfg->theme.bg_panel, value, sizeof(cfg->theme.bg_panel) - 1);
		else if (strcmp(key, "BG_SIDEBAR") == 0)
			strncpy(cfg->theme.bg_sidebar, value, sizeof(cfg->theme.bg_sidebar) - 1);
		else if (strcmp(key, "BG_HOVER") == 0)
			strncpy(cfg->theme.bg_hover, value, sizeof(cfg->theme.bg_hover) - 1);
		else if (strcmp(key, "ACCENT") == 0)
			strncpy(cfg->theme.accent, value, sizeof(cfg->theme.accent) - 1);
		else if (strcmp(key, "ACCENT_ALT") == 0)
			strncpy(cfg->theme.accent_alt, value, sizeof(cfg->theme.accent_alt) - 1);
		else if (strcmp(key, "TEXT_MAIN") == 0)
			strncpy(cfg->theme.text_main, value, sizeof(cfg->theme.text_main) - 1);
		else if (strcmp(key, "TEXT_MUTED") == 0)
			strncpy(cfg->theme.text_muted, value, sizeof(cfg->theme.text_muted) - 1);
		else if (strcmp(key, "TEXT_SUBTLE") == 0)
			strncpy(cfg->theme.text_subtle, value, sizeof(cfg->theme.text_subtle) - 1);
		else if (strcmp(key, "BORDER_SOFT") == 0)
			strncpy(cfg->theme.border_soft, value, sizeof(cfg->theme.border_soft) - 1);
		else if (strcmp(key, "RADIUS_LG") == 0)
			strncpy(cfg->theme.radius_lg, value, sizeof(cfg->theme.radius_lg) - 1);
		else if (strcmp(key, "RADIUS_MD") == 0)
			strncpy(cfg->theme.radius_md, value, sizeof(cfg->theme.radius_md) - 1);
		else if (strcmp(key, "TRANSITION_FAST") == 0)
			strncpy(cfg->theme.transition_fast, value, sizeof(cfg->theme.transition_fast) - 1);
		else if (strcmp(key, "TRANSITION_NORMAL") == 0)
			strncpy(cfg->theme.transition_normal, value, sizeof(cfg->theme.transition_normal) - 1);
	}
	else if (strcmp(parent, "THEME_ICONS") == 0)
	{
		if (strcmp(key, "DEFAULT") == 0)
			cfg->themeIcons.defaultIcon = strdup(value);
		else if (strcmp(key, "DARK") == 0)
			cfg->themeIcons.darkIcon = strdup(value);
		else if (strcmp(key, "LIGHT") == 0)
			cfg->themeIcons.lightIcon = strdup(value);
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