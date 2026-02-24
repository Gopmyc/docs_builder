#include "docs_builder.h"

//
// ┌──────────────────────┐
// │ DOCS GENERATION SCAN │
// └──────────────────────┘
//
int scan_and_create_docs(const char* base, const char* rel, const ProjectConfig* config, ManifestNode* manifest_root)
{
	char searchPath[MAX_PATH];
	if (strlen(rel) == 0)
		snprintf(searchPath, sizeof(searchPath), "%s\\*", base);
	else
		snprintf(searchPath, sizeof(searchPath), "%s\\%s\\*", base, rel);

	WIN32_FIND_DATAA ffd;
	HANDLE hFind = FindFirstFileA(searchPath, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
		return 0;

	int hasValidFile = 0;

	do
	{
		if (!strcmp(ffd.cFileName, ".") || !strcmp(ffd.cFileName, ".."))
			continue;

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			char newRel[MAX_PATH];
			if (strlen(rel) == 0)
				snprintf(newRel, sizeof(newRel), "%s", ffd.cFileName);
			else
				snprintf(newRel, sizeof(newRel), "%s\\%s", rel, ffd.cFileName);

			if (scan_and_create_docs(base, newRel, config, manifest_root))
				hasValidFile = 1;
		}
		else if (strstr(ffd.cFileName, ".ddoc"))
		{
			char newRelFile[MAX_PATH];
			if (strlen(rel) == 0)
				snprintf(newRelFile, sizeof(newRelFile), "%s", ffd.cFileName);
			else
				snprintf(newRelFile, sizeof(newRelFile), "%s\\%s", rel, ffd.cFileName);

			char inputFilePath[MAX_PATH];
			snprintf(inputFilePath, sizeof(inputFilePath), "%s\\%s", base, newRelFile);

			FILE* fIn = fopen(inputFilePath, "r");
			if (!fIn)
				continue;

			ParsedFile* parsed = parse_doc_file(fIn);
			fclose(fIn);
			if (!parsed)
				continue;

			char outputDir[MAX_PATH];
			snprintf(outputDir, sizeof(outputDir), "%s\\pages", config->runtime.output_folder);
			if (strlen(rel) > 0)
			{
				strcat(outputDir, "\\");
				strcat(outputDir, rel);
			}
			create_directory_recursive(outputDir);

			char outputFile[MAX_PATH];
			snprintf(outputFile, sizeof(outputFile), "%s\\pages\\%s", config->runtime.output_folder, newRelFile);
			char* ext = strrchr(outputFile, '.');
			if (ext) strcpy(ext, ".html");

			int depth = 0;
			for (char* p = newRelFile; *p; ++p)
				if (*p == '\\')
					++depth;

			char prefix[64] = {0};
			for (int i = 0; i <= depth; ++i)
				strcat(prefix, "../");

			FILE* fOut = fopen(outputFile, "w");
			if (fOut)
			{
				fprintf(fOut,
					"<!DOCTYPE html>\n"
					"<html lang=\"en\">\n"
					"<head>\n"
					"<meta charset=\"UTF-8\">\n"
					"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
					"<link href=\"%sstyle.css\" rel=\"stylesheet\">\n"
					"<title>%s</title>\n"
					"</head>\n"
					"<body>\n"
					"<div class=\"container\">\n"
					"	<aside class=\"sidebar\"> <h2>📘 %s </h2> <nav> <ul id=\"sidebar-menu\"></ul> </nav> </aside>\n"
					"   <main class=\"content\">\n",
					prefix,
					config->title,
					config->title
				);

				if (parsed->global_description[0])
					fprintf(fOut, "<p>%s</p>\n", parsed->global_description);

				for (DocBlock* d = parsed->blocks; d; d = d->next)
					write_docblock_html(fOut, d, config);

				fprintf(fOut,
					"    </main>\n"
					"</div>\n"
					"<script src=\"%s%s\"></script>\n"
					"<script src=\"%s%s\"></script>\n"
					"<script>window.addEventListener('DOMContentLoaded',()=>{buildSidebarMenu(manifest)});</script>\n"
					"</body>\n"
					"</html>\n",
					prefix, config->runtime.manifest_path,
					prefix, config->runtime.main_js_path
				);

				fclose(fOut);

				char manifestPath[512];
				strncpy(manifestPath, newRelFile, sizeof(manifestPath) - 1);
				char* mext = strrchr(manifestPath, '.');
				if (mext) strcpy(mext, ".html");
				manifest_add_path(manifest_root, manifestPath);

				hasValidFile = 1;
			}

			free_parsed_file(parsed);
		}

	} while (FindNextFileA(hFind, &ffd) != 0);

	FindClose(hFind);
	return hasValidFile;
}