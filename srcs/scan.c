#include "docs_builder.h"

//
// ┌──────────────────────┐
// │ DOCS GENERATION SCAN │
// └──────────────────────┘
//
int scan_and_create_docs(const char* base, const char* rel, const ProjectConfig* config)
{
	char searchPath[MAX_PATH];

	if (strlen(rel) == 0) { snprintf(searchPath, sizeof(searchPath), "%s\\*", base); }
		else { snprintf(searchPath, sizeof(searchPath), "%s\\%s\\*", base, rel); }

	WIN32_FIND_DATAA ffd;
	HANDLE hFind		= FindFirstFileA(searchPath, &ffd);

	if (hFind == INVALID_HANDLE_VALUE) return (0);

	int hasValidFile	= 0;

	do
	{
		if (!strcmp(ffd.cFileName, ".") || !strcmp(ffd.cFileName, "..")) {continue;}

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			char newRel[MAX_PATH];

			if (strlen(rel) == 0) { snprintf(newRel, sizeof(newRel), "%s", ffd.cFileName); }
				else { snprintf(newRel, sizeof(newRel), "%s\\%s", rel, ffd.cFileName); }

			int childHas	= scan_and_create_docs(base, newRel, config);
			if (childHas)
			{
				char fullOutputPath[MAX_PATH];
				snprintf(fullOutputPath, sizeof(fullOutputPath), "%s\\%s", config->runtime.output_folder, newRel);
				create_directory_recursive(fullOutputPath);

				hasValidFile	= 1;
			}
		}
		else
		{
			if (strstr(ffd.cFileName, ".lua"))
			{
				char newRelFile[MAX_PATH];
				if (strlen(rel) == 0) { snprintf(newRelFile, sizeof(newRelFile), "%s", ffd.cFileName); }
					else { snprintf(newRelFile, sizeof(newRelFile), "%s\\%s", rel, ffd.cFileName); }

				char inputFilePath[MAX_PATH];
				snprintf(inputFilePath, sizeof(inputFilePath), "%s\\%s", base, newRelFile);

				if (contains_doc_comment(inputFilePath))
				{
					FILE* fIn		= fopen(inputFilePath, "r");

					if (!fIn) continue;

					DocBlock* docs	= parse_doc_blocks(fIn);
					fclose(fIn);

					char outputDir[MAX_PATH];
					strcpy(outputDir, config->runtime.output_folder);

					if (strlen(rel) > 0) { strcat(outputDir, "\\"); strcat(outputDir, rel); }

					create_directory_recursive(outputDir);

					char outputFile[MAX_PATH];

					strcpy(outputFile, config->runtime.output_folder);
					strcat(outputFile, "\\");
					strcat(outputFile, newRelFile);

					char* ext = strrchr(outputFile, '.');

					if (ext) { *ext = '\0'; }

					strcat(outputFile, ".html");

					FILE* fOut	= fopen(outputFile, "w");
					if (fOut)
					{
						fprintf(fOut,
							"<!DOCTYPE html>\n"
							"<html lang=\"en\">\n"
							"	<head>\n"
							"		<meta charset=\"UTF-8\" />\n"
							"		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n"
							"		<link href=\"https://fonts.googleapis.com/css2?family=Inter:wght@400;700&display=swap\" rel=\"stylesheet\" />\n"
							"	</head>\n"
							"	<body>\n"
							"		<div class=\"container\">\n"
							"			<aside class=\"sidebar\"></aside>\n"
							"			<main class=\"content\">\n"
						);

						for (DocBlock* d = docs; d; d = d->next) { write_docblock_html(fOut, d); }

						fprintf(fOut,
							"			</main>\n"
							"		</div>\n"
							"		<script>(()=>{let p=window.location.pathname;p=p.endsWith('/')?p.slice(0,-1):p;const a=p.split('/').filter(Boolean),i=a.findIndex(e=>e.toLowerCase()=='docs'),d=i>=0?a.length-(i+1):a.length-1,b='../'.repeat(d>0?d:0),s=b+'docs/style.css',j=b+'docs/main.js',l=document.createElement('link');l.rel='stylesheet';l.href=s;document.head.appendChild(l);window.addEventListener('DOMContentLoaded',()=>{const c=document.createElement('script');c.src=j;document.body.appendChild(c);});})();</script>\n"
							"	</body>\n"
							"</html>\n"
						);

						fclose(fOut);
					}

					free_docblocks(docs);
					hasValidFile	= 1;
				}
			}
		}
	} while (FindNextFileA(hFind, &ffd));

	FindClose(hFind);
	return (hasValidFile);
}
