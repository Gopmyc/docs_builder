#include "docs_builder.h"

//
// ┌──────┐
// │ MAIN │
// └──────┘
//

// TODO: Create a version control system, a search bar, and a client/server differentiation system
#include "docs_builder.h"

static void extract_parent_dir(const char* fullPath, char* outDir, size_t outSize)
{
	const char* jsPos = strstr(fullPath, ".js");
	if (!jsPos) { outDir[0] = '\0'; return; }

	const char* slashPos = jsPos;
	while (slashPos > fullPath && *slashPos != '/' && *slashPos != '\\')
		slashPos--;

	size_t len = (slashPos >= fullPath) ? (size_t)(slashPos - fullPath) : 0;
	if (len >= outSize) len = outSize - 1;

	memcpy(outDir, fullPath, len);
	outDir[len] = '\0';
}

int main(void)
{
	ProjectConfig config;

	if (!load_config_yaml("docs_config.yaml", &config))
		return (printf("[ERROR] Failed to load 'docs_config.yaml'\n"), 1);

	set_runtime_config(&config.runtime);
	
	logInfo("Creating output folder", config.runtime.output_folder);
	create_directory_recursive(config.runtime.output_folder);
	logSuccess("Output folder ready at", config.runtime.output_folder);

	ManifestNode* manifest_root = create_node("", 0);

	logInfo("Starting docs generation", NULL);

	int result = scan_and_create_docs(
		config.runtime.input_folder,
		"",
		&config,
		manifest_root);

	if (!result)
		logError("No .ddoc files found with doc comments", NULL);

	logInfo("Generating manifest", NULL);

	char manifestPath[4096];
	snprintf(manifestPath, sizeof(manifestPath),
		"%s\\%s",
		config.runtime.output_folder,
		config.runtime.manifest_path);

	char manifestDir[4096];
	extract_parent_dir(manifestPath, manifestDir, sizeof(manifestDir));
	if (manifestDir[0] != '\0')
		create_directory_recursive(manifestDir);

	FILE* fOut = fopen(manifestPath, "w");
	if (!fOut)
		return (logError("Failed to create manifest file", manifestPath), 1);

	fprintf(fOut, "const manifest = {\n");
	write_manifest(fOut, manifest_root, 1, &config);
	fprintf(fOut, "\n};\n");

	fclose(fOut);
	logSuccess("Manifest generated at", manifestPath);

	free_manifest(manifest_root);

	logInfo("Generating index.html", NULL);
	generate_index_html(&config);

	logInfo("Generating style.css", NULL);
	generate_style_css(&config);

	logInfo("Generating main.js", NULL);
	generate_main_js(&config);

	logSuccess("Docs build complete", config.runtime.output_folder);

	return 0;
}