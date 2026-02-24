#pragma once

//
// ┌─────────────┐
// │ BASE MACROS │
// └─────────────┘
//
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

//
// ┌───────────┐
// │ LIBRARIES │
// └───────────┘
//
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>

#define MAX_LINE		1024

//
// ┌───────────────┐
// │ CUSTOM STRUCT │
// └───────────────┘
//
typedef struct Param
{
	char			type[64];
	char			desc[256];
	struct Param*	next;
} Param;

typedef struct Return
{
	char			type[64];
	char			desc[256];
	struct Return*	next;
} Return;

typedef struct DocBlock
{
	char			name[128];
	char			desc[1024];

	char			example_lang[32];
	char			example_code[4096];

	char			state[16];

	Param			*params;
	Return			*returns;

	struct DocBlock	*next;
} DocBlock;

typedef struct ParsedFile
{
	char		global_description[4096];
	DocBlock	*blocks;
} ParsedFile;

typedef struct ThemeConfig
{
	char	mode[16];
	char	bg_main[16];
	char	bg_gradient_start[16];
	char	bg_gradient_end[16];
	char	bg_panel[16];
	char	bg_sidebar[16];
	char	bg_hover[16];
	char	accent[16];
	char	accent_alt[16];
	char	text_main[16];
	char	text_muted[16];
	char	text_subtle[16];
	char	border_soft[16];
	char	radius_lg[8];
	char	radius_md[8];
	char	transition_fast[16];
	char	transition_normal[16];
} ThemeConfig;

typedef struct ThemeIcons {
    const char	*defaultIcon;
    const char	*darkIcon;
    const char	*lightIcon;
} ThemeIcons;

typedef struct RuntimeConfig
{
	char	input_folder[256];
	char	output_folder[256];
	char	root_path[256];
	char	extension[32];
	char	exclude_file[128];
	int 	indent_width;
	char	manifest_path[256];
	char	main_js_path[256];

	int 	color_reset;
	int 	color_success;
	int 	color_error;
	int 	color_info;

	char 	color_client[16];
	char 	color_server[16];

} RuntimeConfig;

typedef struct ProjectConfig
{
	char			title[128];
	char			description[512];
	char			license_name[128];
	char			license_url[256];

	ThemeConfig		theme;
	ThemeIcons		themeIcons;

	RuntimeConfig	runtime;

} ProjectConfig;

typedef struct ManifestNode
{
	char				name[256];
	int					is_file;
	struct ManifestNode	*children;
	struct ManifestNode	*next;
} ManifestNode;

//
// ┌──────────────────┐
// │ UTILITY FUNCTION │
// └──────────────────┘
//
void			writeIndent(FILE *fOut, int iDepth, const ProjectConfig* config);
int				endsWithHtml(const char *sName, const ProjectConfig* config);
int				isDirectory(const char *sPath);

//
// ┌──────────────┐
// │ LOG FUNCTION │
// └──────────────┘
//
void			set_runtime_config(RuntimeConfig* runtime);
void			setConsoleColor(WORD wColor);
void			logError(const char *sMsg, const char *sDetail);
void			logInfo(const char *sMsg, const char *sDetail);
void			logSuccess(const char *sMsg, const char *sDetail);


//
// ┌──────────────────────────┐
// │ DOCS GENERATION FUNCTION │
// └──────────────────────────┘
//
void			free_docblocks(DocBlock* head);
void			create_directory_recursive(const char* path);
ParsedFile*		parse_doc_file(FILE* f);
void			free_parsed_file(ParsedFile* file);
void			write_docblock_html(FILE* fOut, DocBlock* doc, const ProjectConfig* config);
int				scan_and_create_docs(const char* base, const char* rel, const ProjectConfig* config, ManifestNode* manifest_root);

//
// ┌──────────────────────────────┐
// │ MANIFEST GENERATION FUNCTION │
// └──────────────────────────────┘
//
ManifestNode*	create_node(const char* name, int is_file);
void			manifest_add_path(ManifestNode* root, const char* path);
void			write_manifest(FILE* f, ManifestNode* node, int depth, const ProjectConfig* config);
void			free_manifest(ManifestNode* node);

//
// ┌─────────────────────────────────┐
// │ HTML/JS/CSS GENERATION FUNCTION │
// └─────────────────────────────────┘
//
void			generate_index_html(const ProjectConfig* config);
void			generate_main_js(const ProjectConfig* config);
void			generate_style_css(const ProjectConfig* config);

//
// ┌───────────────┐
// │ YAML FUNCTION │
// └───────────────┘
//
int				load_config_yaml(const char* path, ProjectConfig* config);