#include "docs_builder.h"

//
// ┌────────────────────────┐
// │ DOCS GENERATION WRITER │
// └────────────────────────┘
//
void write_docblock_html(FILE* fOut, DocBlock* doc)
{
	fprintf(fOut,
		"<section id=\"%s\">\n"
		"	<h2>%s</h2>\n"
		"	<p>\n"
		"		<li><strong>@desc :</strong></li>\n"
		"		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; %s\n"
		"	</p>\n"
		"	<ul class=\"meta\">\n",
		doc->name[0] ? doc->name : "unnamed",
		doc->name[0] ? doc->name : "Unnamed",
		doc->desc[0] ? doc->desc : "No description."
	);

	if (doc->params) {
		fprintf(fOut, "		<li><strong>@param :</strong></li>\n");
		for (Param* p = doc->params; p; p = p->next)
			fprintf(fOut, "		<li>&nbsp;&nbsp;&nbsp;&nbsp;<b>{%s}</b> : %s</li>\n", p->type, p->desc);
	}

	if (doc->returns) {
		fprintf(fOut, "		<li><strong>@return :</strong></li>\n");
		for (Return* r = doc->returns; r; r = r->next)
			fprintf(fOut, "		<li>&nbsp;&nbsp;&nbsp;&nbsp;<b>{%s}</b> : %s</li>\n", r->type, r->desc);
	}

	fprintf(fOut,
		"	</ul>\n"
		"	<div class=\"code-block\">\n"
		"		<button class=\"copy-btn\">📋</button>\n"
		"		<pre><code>\n"
		"		-- Some Code\n"
		"		</code></pre>\n"
		"	</div>\n"
		"</section>\n"
	);
}

//
// ┌────────────────────────────┐
// │ TREE GENERATOR (RECURSIVE) │
// └────────────────────────────┘
//
void	writeTree(FILE *fOut, const char *sPath, int iDepth)
{
	struct _finddata_t sEntry;
	char sSearch[4096];
	char sFullPath[4096];
	long hFind;
	int bFirst = 1;

	sprintf(sSearch, "%s\\*", sPath);
	hFind = _findfirst(sSearch, &sEntry);
	if (hFind == -1L)
	{
		logError("Directory listing failed", sPath);
		return;
	}

	do
	{
		if (strcmp(sEntry.name, ".") == 0 || strcmp(sEntry.name, "..") == 0)
			continue;

		sprintf(sFullPath, "%s\\%s", sPath, sEntry.name);

		if (isDirectory(sFullPath))
		{
			if (!bFirst)
				fprintf(fOut, ",\n");
			bFirst = 0;
			writeIndent(fOut, iDepth);
			fprintf(fOut, "\"%s\": {\n", sEntry.name);
			writeTree(fOut, sFullPath, iDepth + 1);
			fprintf(fOut, "\n");
			writeIndent(fOut, iDepth);
			fprintf(fOut, "}");
		}
		else if (endsWithHtml(sEntry.name))
		{
			if (!bFirst)
				fprintf(fOut, ",\n");
			bFirst = 0;
			writeIndent(fOut, iDepth);
			fprintf(fOut, "\"%s\": null", sEntry.name);
		}
	}
	while (_findnext(hFind, &sEntry) == 0);

	_findclose(hFind);
}
