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
		"<h2>%s</h2>\n"
		"<p>%s</p>\n",
		doc->name[0] ? doc->name : "unnamed",
		doc->name[0] ? doc->name : "Unnamed",
		doc->desc[0] ? doc->desc : "No description."
	);

	if (doc->params)
	{
		fprintf(fOut, "<h3>Parameters</h3><ul>");
		for (Param* p = doc->params; p; p = p->next)
			fprintf(fOut, "<li><b>{%s}</b> : %s</li>", p->type, p->desc);
		fprintf(fOut, "</ul>");
	}

	if (doc->returns)
	{
		fprintf(fOut, "<h3>Returns</h3><ul>");
		for (Return* r = doc->returns; r; r = r->next)
			fprintf(fOut, "<li><b>{%s}</b> : %s</li>", r->type, r->desc);
		fprintf(fOut, "</ul>");
	}

	if (doc->example_code[0])
	{
		fprintf(fOut,
			"<div class=\"code-block\">"
			"<pre><code class=\"language-%s\">%s</code></pre>"
			"</div>",
			doc->example_lang,
			doc->example_code
		);
	}

	fprintf(fOut, "</section>\n");
}

//
// ┌────────────────────────────┐
// │ TREE GENERATOR (RECURSIVE) │
// └────────────────────────────┘
//
void writeTree(FILE *fOut, const char *sPath, int iDepth, const ProjectConfig* config)
{
	struct _finddata_t sEntry;
	char sSearch[4096];
	char sFullPath[4096];
	long hFind;
	int bFirst = 1;

	sprintf(sSearch, "%s\\*", sPath);
	hFind = _findfirst(sSearch, &sEntry);
	if (hFind == -1L) return;

	do
	{
		if (strcmp(sEntry.name, ".") == 0 || strcmp(sEntry.name, "..") == 0)
			continue;

		sprintf(sFullPath, "%s\\%s", sPath, sEntry.name);

		if (isDirectory(sFullPath))
		{
			if (!bFirst) fprintf(fOut, ",\n");
			bFirst = 0;
			writeIndent(fOut, iDepth, config);
			fprintf(fOut, "\"%s\": {\n", sEntry.name);
			writeTree(fOut, sFullPath, iDepth + 1, config);
			fprintf(fOut, "\n");
			writeIndent(fOut, iDepth, config);
			fprintf(fOut, "}");
		}
		else if (endsWithHtml(sEntry.name, config))
		{
			if (!bFirst) fprintf(fOut, ",\n");
			bFirst = 0;
			writeIndent(fOut, iDepth, config);
			fprintf(fOut, "\"%s\": null", sEntry.name);
		}
	} while (_findnext(hFind, &sEntry) == 0);

	_findclose(hFind);
}