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

//
// ┌───────────────┐
// │ CONFIG MACROS │
// └───────────────┘
//
#define INPUT_FOLDER	"lua"
#define OUTPUT_FOLDER	"docs\\root"
#define MAX_LINE		1024
#define EXTENSION		".html"
#define EXCLUDE_FILE	"index.html"
#define OUTPUT_FILE		"manifest.js"
#define ROOT_PATH		"docs/root"
#define INDENT_WIDTH	1

//
// ┌───────────────────┐
// │ COLOR DEFINITIONS │
// └───────────────────┘
//
#define COLOR_RESET		7
#define COLOR_SUCCESS	10
#define COLOR_ERROR		12
#define COLOR_INFO		11

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
	char				name[128];
	char				desc[512];
	Param*				params;
	Return*				returns;
	struct DocBlock*	next;
} DocBlock;

typedef struct ThemeConfig
{
	char mode[16];
	char bg_main[16];
	char bg_gradient_start[16];
	char bg_gradient_end[16];
	char bg_panel[16];
	char bg_sidebar[16];
	char bg_hover[16];
	char accent[16];
	char accent_alt[16];
	char text_main[16];
	char text_muted[16];
	char text_subtle[16];
	char border_soft[16];
	char radius_lg[8];
	char radius_md[8];
	char transition_fast[16];
	char transition_normal[16];
} ThemeConfig;

typedef struct ThemeIcons {
    const char* defaultIcon;
    const char* darkIcon;
    const char* lightIcon;
} ThemeIcons;

typedef struct ProjectConfig
{
	char title[128];
	char description[512];
	char license_name[128];
	char license_url[256];
	ThemeConfig theme;
	ThemeIcons themeIcons;
} ProjectConfig;

//
// ┌──────────────────┐
// │ UTILITY FUNCTION │
// └──────────────────┘
//
void		writeIndent(FILE *fOut, int iDepth);
int			endsWithHtml(const char *sName);
int			isDirectory(const char *sPath);

//
// ┌──────────────┐
// │ LOG FUNCTION │
// └──────────────┘
//
void		setConsoleColor(WORD wColor);
void		logError(const char *sMsg, const char *sDetail);
void		logInfo(const char *sMsg, const char *sDetail);
void		logSuccess(const char *sMsg, const char *sDetail);


//
// ┌──────────────────────────┐
// │ DOCS GENERATION FUNCTION │
// └──────────────────────────┘
//
void		free_docblocks(DocBlock* head);
void		create_directory_recursive(const char* path);
DocBlock*	parse_doc_blocks(FILE* f);
int			contains_doc_comment(const char* filepath);
void		write_docblock_html(FILE* fOut, DocBlock* doc);
int			scan_and_create_docs(const char* base, const char* rel);

//
// ┌──────────────────────────────┐
// │ MANIFEST GENERATION FUNCTION │
// └──────────────────────────────┘
//
void		writeTree(FILE *fOut, const char *sPath, int iDepth);

//
// ┌─────────────────────────────────┐
// │ HTML/JS/CSS GENERATION FUNCTION │
// └─────────────────────────────────┘
//
void		generate_index_html(const ProjectConfig* config);
void		generate_main_js(const ProjectConfig* config);
void		generate_style_css(const ProjectConfig* config);

//
// ┌───────────────┐
// │ YAML FUNCTION │
// └───────────────┘
//
int			load_config_yaml(const char* path, ProjectConfig* config);