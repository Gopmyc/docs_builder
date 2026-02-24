#include "docs_builder.h"

ManifestNode* create_node(const char* name, int is_file)
{
	ManifestNode* node = malloc(sizeof(ManifestNode));
	if (!node)
		return NULL;

	strncpy(node->name, name, sizeof(node->name) - 1);
	node->name[sizeof(node->name) - 1] = '\0';

	node->is_file = is_file;
	node->children = NULL;
	node->next = NULL;

	return node;
}

ManifestNode* find_child(ManifestNode* parent, const char* name)
{
	ManifestNode* child = parent->children;

	while (child)
	{
		if (strcmp(child->name, name) == 0)
			return child;
		child = child->next;
	}

	return NULL;
}

static void add_child(ManifestNode* parent, ManifestNode* child)
{
	if (!parent->children)
		parent->children = child;
	else
	{
		ManifestNode* tmp = parent->children;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = child;
	}
}

void manifest_add_path(ManifestNode* root, const char* path)
{
	if (!root || !path || !*path)
		return;

	char buffer[512];
	strncpy(buffer, path, sizeof(buffer) - 1);
	buffer[sizeof(buffer) - 1] = '\0';

	char* token = strtok(buffer, "\\/");
	ManifestNode* current = root;

	while (token)
	{
		ManifestNode* child = find_child(current, token);

		if (!child)
		{
			int isFile = strchr(token, '.') != NULL;
			child = create_node(token, isFile);
			add_child(current, child);
		}

		current = child;
		token = strtok(NULL, "\\/");
	}
}

void write_manifest(FILE* f, ManifestNode* node, int depth, const ProjectConfig* config)
{
	int first = 1;

	for (ManifestNode* c = node->children; c; c = c->next)
	{
		if (!first)
			fprintf(f, ",\n");
		first = 0;

		writeIndent(f, depth, config);

		if (c->is_file)
		{
			fprintf(f, "\"%s\": null", c->name);
		}
		else
		{
			fprintf(f, "\"%s\": {\n", c->name);
			write_manifest(f, c, depth + 1, config);
			fprintf(f, "\n");
			writeIndent(f, depth, config);
			fprintf(f, "}");
		}
	}
}

void free_manifest(ManifestNode* node)
{
	if (!node) return;

	ManifestNode* c = node->children;
	while (c)
	{
		ManifestNode* next = c->next;
		free_manifest(c);
		c = next;
	}

	free(node);
}