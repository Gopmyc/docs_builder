#include "docs_builder.h"

//
// ┌────────────────────────┐
// │ DOCS GENERATION WRITER │
// └────────────────────────┘
//
void write_docblock_html(FILE* fOut, DocBlock* doc)
{
	char stateHtml[256] = "";
	if (doc->state[0])
	{
		if (strcmp(doc->state, "CLIENT") == 0)
			strcpy(stateHtml, "<span style='display:inline-block;width:12px;height:12px;background:#f1c40f;border-radius:6px;margin-right:8px;'></span>");
		else if (strcmp(doc->state, "SERVER") == 0)
			strcpy(stateHtml, "<span style='display:inline-block;width:12px;height:12px;background:#3498db;border-radius:6px;margin-right:8px;'></span>");
		else if ((strcmp(doc->state, "CLIENT/SERVER") == 0) || (strcmp(doc->state, "SERVER/CLIENT") == 0))
	strcpy(stateHtml,
		"<span style='display:inline-block;width:12px;height:12px;margin-right:8px;border-radius:6px;"
		"background:linear-gradient(135deg,#f1c40f 50%,#3498db 50%)'></span>");
	}

	fprintf(fOut,
		"<section id=\"%s\">\n"
		"<h2>%s %s</h2>\n"
		"<p>%s</p>\n",
		doc->name[0] ? doc->name : "unnamed",
		stateHtml,
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