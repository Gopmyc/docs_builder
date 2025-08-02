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

//
// ┌──────────────────┐
// │ UTILITY FUNCTION │
// └──────────────────┘
//
void		writeIndent(FILE *fOut, int iDepth);
int			endsWithHtml(const char *sName);
int			isDirectory(const char *sPath);
