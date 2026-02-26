#include "docs_builder.h"

//
// ┌────────────────────────┐
// │ DOCS GENERATION WRITER │
// └────────────────────────┘
//
const char* getParamTypeClass(const char* type)
{
	if (!type || !type[0]) return "default";
	static const char* types[] = { "string", "table", "number", "boolean", "function", "any" };
	for (int i = 0; i < sizeof(types)/sizeof(types[0]); i++)
		if (strcmp(type, types[i]) == 0)
			return types[i];
	return "default";
}

void write_docblock_html(FILE* fOut, DocBlock* doc, const ProjectConfig* config)
{
	char stateHtml[256] = "";
	if (doc->state[0])
	{
		if (strcmp(doc->state, "CLIENT") == 0)
			snprintf(stateHtml, sizeof(stateHtml), "<span class='state-dot' style='background:%s;'></span>", config->runtime.color_client);
		else if (strcmp(doc->state, "SERVER") == 0)
			snprintf(stateHtml, sizeof(stateHtml), "<span class='state-dot' style='background:%s;'></span>", config->runtime.color_server);
		else if ((strcmp(doc->state, "CLIENT/SERVER") == 0) || (strcmp(doc->state, "SERVER/CLIENT") == 0))
			snprintf(stateHtml, sizeof(stateHtml), "<span class='state-dot' style='background:linear-gradient(135deg,%s 50%%,%s 50%%);'></span>", config->runtime.color_client, config->runtime.color_server);
	}

	fprintf(fOut, "<section class='doc-block' id='%s'>\n<h2>%s %s</h2>\n<p>%s</p>\n",
		doc->name[0] ? doc->name : "unnamed",
		stateHtml,
		doc->name[0] ? doc->name : "Unnamed",
		doc->desc[0] ? doc->desc : "No description."
	);

	if (doc->params)
	{
		fprintf(fOut, "<div class='doc-subblock'><h3>Parameters</h3><ul>");
		for (Param* p = doc->params; p; p = p->next)
		{
			const char* cssClass = getParamTypeClass(p->type);
			fprintf(fOut, "<li><span class='param-type %s'>%s</span> : %s</li>", cssClass, p->type, p->desc);
		}
		fprintf(fOut, "</ul></div>");
	}

	if (doc->returns)
	{
		fprintf(fOut, "<div class='doc-subblock'><h3>Returns</h3><ul>");
		for (Return* r = doc->returns; r; r = r->next)
		{
			const char* cssClass = getParamTypeClass(r->type);
			fprintf(fOut, "<li><span class='param-type %s'>%s</span> : %s</li>", cssClass, r->type, r->desc);
		}
		fprintf(fOut, "</ul></div>");
	}

	if (doc->example_code[0])
	{
		fprintf(fOut, "<div class='doc-subblock code-block'><pre><code class='language-%s'>%s</code></pre></div>",
			doc->example_lang, doc->example_code
		);
	}

	fprintf(fOut, "</section>\n");
}