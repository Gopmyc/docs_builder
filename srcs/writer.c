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
